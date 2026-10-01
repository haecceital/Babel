import numpy as np


size2d = tuple[int, int]

EPS = 1E-8
DECAY_RATE = 0.9
LR_MAX = 0.01
LR_MIN = 1E-6
TOTAL_EPOCHS = 100


sliding_window_view = np.lib.stride_tricks.sliding_window_view

# cosine annealing
def get_cosine_lr(epoch: int):
    if epoch >= TOTAL_EPOCHS:
        return LR_MIN

    p = epoch / TOTAL_EPOCHS

    val = np.cos(p * np.pi)

    cur_lr = LR_MIN + 0.5 * (LR_MAX - LR_MIN) * (1 + val)

    return cur_lr


def simple_im2col(image: np.ndarray, kernel_size: size2d, stride = 1) -> np.ndarray:
    kernel_h, kernel_w = kernel_size

    channels, img_h, img_w = image.shape

    out_h = (img_h - kernel_h) // stride + 1
    out_w = (img_w - kernel_w) // stride + 1

    cols = []

    for y in range(0, img_h - kernel_h + 1, stride):
        for x in range(0, img_w - kernel_w + 1, stride):
            tmp = image[:, y:y + kernel_h, x:x + kernel_w]

            cols.append(tmp.reshape(-1, 1))

    image_matrix = np.hstack(cols)

    return image_matrix, (out_h, out_w)


def im2col(image: np.ndarray, kernel_size: size2d, stride:int = 1) -> np.ndarray:
    kernel_h, kernel_w = kernel_size
    channels, img_h, img_w = image.shape

    out_h = (img_h - kernel_h) // stride + 1
    out_w = (img_w - kernel_w) // stride + 1

    windows = sliding_window_view(image, window_shape = kernel_size, axis = (1, 2))

    strided_windows = windows[:, ::stride, ::stride, :, :]

    transposed = strided_windows.transpose(0, 3, 4, 1, 2)

    image_matrix = transposed.reshape(channels * kernel_h * kernel_w, out_h * out_w)

    return image_matrix, (out_h, out_w)


def simple_col2im(d_data_matrix: np.ndarray, data_shape: tuple, kernel_size: size2d, stride: int = 1):
    d_input = np.zeros(data_shape)

    channels, height, width = data_shape
    kernel_h, kernel_w = kernel_size

    out_h = (height - kernel_h) // stride + 1
    out_w = (width - kernel_w) // stride + 1

    col_idx = 0

    for y in range(out_h):
        for x in range(out_w):
            h_start = y * stride
            h_end = h_start + kernel_h
            w_start = x * stride
            w_end = w_start + kernel_w

            tmp = d_data_matrix[:, col_idx].reshape(channels, kernel_h, kernel_w)

            d_input[:, h_start:h_end, w_start:w_end] += tmp

            col_idx += 1

    return d_input


class Conv2D:
    def __init__(self, in_channels: int, out_channels: int, kernel_size: size2d = (3, 3)):
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = kernel_size

        self.filters = np.random.randn(out_channels, in_channels, *kernel_size) * 0.01
        self.biases = np.zeros(out_channels)

        self.cache_d_filters = np.zeros_like(self.filters)
        self.cache_d_biases = np.zeros_like(self.biases)


    def forward(self, data: np.ndarray) -> np.ndarray:
        filters_matrix = self.filters.reshape(self.out_channels, -1)
        data_matrix, output_size = im2col(data, self.kernel_size, 1)

        self.cache = {
            "data_matrix": data_matrix,
            "data_shape": data.shape
        }

        output_matrix = (filters_matrix @ data_matrix) + self.biases.reshape(-1, 1)

        output = output_matrix.reshape(self.out_channels, *output_size)

        return output


    def backward(self, d_out: np.ndarray, learning_rate: float = 0.001) -> np.ndarray:
        d_out_matrix = d_out.reshape(self.out_channels, -1)

        d_biases = np.sum(d_out_matrix, axis = 1)

        d_filters_matrix = d_out_matrix @ self.cache["data_matrix"].T
        d_filters = d_filters_matrix.reshape(self.filters.shape)

        filters_matrix = self.filters.reshape(self.out_channels, -1)

        d_data_matrix = filters_matrix.T @ d_out_matrix

        d_input = simple_col2im(d_data_matrix, self.cache["data_shape"], self.kernel_size)

        # RMSprop algorithm
        self.cache_d_filters = DECAY_RATE * self.cache_d_filters + (1 - DECAY_RATE) * (d_filters ** 2)
        self.cache_d_biases = DECAY_RATE * self.cache_d_biases + (1 - DECAY_RATE) * (d_biases ** 2)

        lr_filters = learning_rate / (np.sqrt(self.cache_d_filters) + EPS)
        lr_biases = learning_rate / (np.sqrt(self.cache_d_biases) + EPS)

        self.filters = self.filters - (lr_filters * d_filters)
        self.biases = self.biases - (lr_biases * d_biases)

        return d_input


class LeakyReLU:
    def __init__(self, alpha: float = 0.01):
        self.alpha = alpha
        self.cache = None


    def forward(self, data: np.ndarray) -> np.ndarray:
        self.cache = (data > 0)

        output = np.maximum(self.alpha * data, data)

        return output


    def backward(self, d_out: np.ndarray, learning_rate: float = None) -> np.ndarray:
        d_input = np.array(d_out, copy = True)

        d_input[~self.cache] *= self.alpha

        return d_input


class MaxPooling2D:
    def __init__(self, pool_size: size2d = (2, 2), stride: int = 2):
        self.cache = None
        self.pool_size = pool_size
        self.stride = stride


    def forward(self, data: np.ndarray) -> np.ndarray:
        windows = sliding_window_view(data, window_shape = self.pool_size, axis = (1, 2))

        strided_windows = windows[:, ::self.stride, ::self.stride, :, :]

        output = np.max(strided_windows, axis = (-2, -1))
        self.cache = data

        return output

    def backward(self, d_out: np.ndarray, learning_rate: float = None) -> np.ndarray:
        d_input = np.zeros_like(self.cache)

        channels, out_h, out_w = d_out.shape
        pool_h, pool_w = self.pool_size

        for c in range(channels):
            for y in range(out_h):
                for x in range(out_w):
                    h_start = y * self.stride
                    h_end = h_start + pool_h
                    w_start = x * self.stride
                    w_end = w_start + pool_w

                    window = self.cache[c, h_start:h_end, w_start:w_end]
                    max_y, max_x = np.unravel_index(np.argmax(window), self.pool_size)

                    d_input[c, h_start + max_y, w_start + max_x] = d_out[c, y, x]

        return d_input


class Flatten:
    def __init__(self):
        self.cache = None


    def forward(self, data: np.ndarray) -> np.ndarray:
        self.cache = data.shape

        output = data.flatten()

        return output


    def backward(self, d_out: np.ndarray, learning_rate: float = None) -> np.ndarray:
        d_input = d_out.reshape(self.cache)

        return d_input


class Dense:
    def __init__(self, in_channels: int, out_channels: int):
        self.in_channels = in_channels
        self.out_channels = out_channels

        self.weights = np.random.randn(out_channels, in_channels) * 0.01
        self.biases = np.zeros(out_channels)

        self.cache_d_weights = np.zeros_like(self.weights)
        self.cache_d_biases = np.zeros_like(self.biases)


    def forward(self, data: np.ndarray) -> np.ndarray:
        self.cache = data

        output = (self.weights @ data) + self.biases

        return output


    def backward(self, d_out: np.ndarray, learning_rate: float = 0.001) -> np.ndarray:
        d_weights = np.outer(d_out, self.cache)

        d_biases = d_out

        d_input = self.weights.T @ d_out

        # RMSprop algorithm
        self.cache_d_weights = DECAY_RATE * self.cache_d_weights + (1 - DECAY_RATE) * (d_weights ** 2)
        self.cache_d_biases = DECAY_RATE * self.cache_d_biases + (1 - DECAY_RATE) * (d_biases ** 2)

        lr_weights = learning_rate / (np.sqrt(self.cache_d_weights) + EPS)
        lr_biases = learning_rate / (np.sqrt(self.cache_d_biases) + EPS)

        self.weights = self.weights - (lr_weights * d_weights)
        self.biases = self.biases - (lr_biases * d_biases)

        return d_input


class SoftmaxCrossEntropy:
    def __init__(self):
        self.cache_y_pred = None
        self.cache_y_true = None

    def forward(self, logits: np.ndarray, y_true: np.ndarray) -> float:
        shifted_logits = logits - np.max(logits)

        exps = np.exp(shifted_logits)
        y_pred = exps / np.sum(exps)

        self.cache_y_pred = y_pred
        self.cache_y_true = y_true

        y_pred_clipped = np.clip(y_pred, EPS, 1 - EPS)
        loss = -np.sum(y_true * np.log(y_pred_clipped))

        return loss

    def backward(self) -> np.ndarray:
        d_out = self.cache_y_pred - self.cache_y_true

        return d_out

pipeline = []

conv1, conv2 = Conv2D(1, 16), Conv2D(16, 32)
relu1, relu2 = LeakyReLU(), LeakyReLU()
pool1, pool2 = MaxPooling2D(), MaxPooling2D()
flatten = Flatten()
dense = Dense(800, 10)
loss_fn = SoftmaxCrossEntropy()

pipeline = [conv1, relu1, pool1, conv2, relu2, pool2, flatten, dense]