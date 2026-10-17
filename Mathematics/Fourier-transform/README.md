# Fourier Transform

傅立葉變換的公式為

$$
f(\xi) = \int_{-\infty}^{\infty} g(t) \cdot e^{-2i \pi \xi t} dt
$$

透過 $e^{ix} = \cos x + i\sin x$ 可以得知 $e^{-2i\pi\xi t}$ 是複平面單位圓上的一個點。再回頭看前面的積分，應該不難看出它會將 $g$ 上的所有點分別乘上一個複平面單位圓上的點，接著全部加起來

直接讓 $g = \sin$，這時候其實這個積分不會收斂，而是會在一個區間內震盪，或是直接發散到無窮大。具體會發散還是震盪，則是由 $e^{-2i \pi \xi t}$ 的旋轉週期決定：如果它的旋轉週期和 $\sin$ 自身的週期對不上，那麼積分結果就是震盪，反之則是發散。除了可以直接把這個積分算出來，也可以透過直覺大概猜出來。因為頻率對不上，如果旋轉頻率和 $\sin$ 頻率相差有理數倍（除了 $\pm 1$），那麼每一循環中同振幅（同長度）的複數會相消，而最後因為旋轉不會停，導致我們不可能知道它剩下哪些沒被消掉，又因為它一圈的值是有限的，所以結果就是在一個範圍內震盪。如果相差是無理數，那永遠都不會陷入循環，但所有同振幅的複數會均勻地遍佈在圓上，所以一樣會相消，而最後我們一樣不能確定哪些沒有被消掉，所以一樣是震盪

若是旋轉頻率和 $\sin$ 頻率完全重合（例如取 $\xi = -\frac{1}{2\pi}$，此時 $e^{-2i\pi\xi t} = e^{it}$），積分結果則是發散，我們可以直接帶一些值進去看：

$$
\begin{aligned}
t = 0: &\quad \sin t = 0,\ e^{it} = 1,\ \sin t \cdot e^{it} = 0 \\
t = \frac{\pi}{2}: &\quad \sin t = 1,\ e^{it} = i,\ \sin t \cdot e^{it} = i \\
t = \pi: &\quad \sin t = 0,\ e^{it} = -1,\ \sin t \cdot e^{it} = 0 \\
t = \frac{3\pi}{2}: &\quad \sin t = -1,\ e^{it} = -i,\ \sin t \cdot e^{it} = i
\end{aligned}
$$

可以看得出來，實數分量會被抵銷，但虛數分量不會，而是會一直疊加，發散至無窮。也可以直接展開確認： $\sin t \cdot e^{it} = \frac{1}{2}\sin 2t + i\sin^2 t$，實部正負震盪而相消，虛部 $\sin^2 t \ge 0$ 則永遠不會抵消

如果想要使這個積分收斂，我們可以把 $g = \sin$ 換成 $g = e^{-\epsilon |t|} \sin t, \, \epsilon > 0$，這時積分必定收斂。考慮 $\epsilon = 1$，這時候 $\lim_{t \to \infty} g(t) = 0$（ $-\infty$ 同理）。此時積分結果會是一個有兩個高峰的函數（位於 $\pm\frac{1}{2\pi}$ 附近），而隨著 $\epsilon$ 從正方向慢慢趨近於 0（ $\lim_{\epsilon \to 0^+}$ ），這兩個尖峰會越來越高、越來越窄，周圍的高度則越來越低，直到 $\lim_{\epsilon \to 0^+}$ 時，在 $\pm\frac{1}{2\pi}$ 的值為無窮大，寬度卻是無窮小。過程中，構成這兩個尖峰的每一項，面積都固定為 $\frac{1}{2}$

而這個高度無窮大、寬度無窮小、面積固定的東西，就是狄拉克 $\delta$ 函數（嚴格來說是廣義函數）。可以簡單理解為 $\delta(0) = \infty$ ， $\delta(r) = 0, \, r \neq 0$ ，且 $\int_{-\infty}^{\infty} \delta(t) \, dt = 1$ 。因此：

$$
\lim_{\epsilon \to 0^+} f(\xi) = -\frac{i}{2}\left[\delta\left(\xi - \frac{1}{2\pi}\right) - \delta\left(\xi + \frac{1}{2\pi}\right)\right]
$$

也就是說， $\sin$ 裡有兩個旋轉頻率（ $e^{it}$ 與 $e^{-it}$ ），變換之後就在這兩個頻率上各留下一個 $\delta$。對一般函數同理，函數裡含有的每個頻率成分，都會在變換結果的對應位置留下一個尖峰，這就是傅立葉變換能找出一個函數由哪些頻率組成的原因


## DFT

但是現實世界我們不一定能夠知道完整的g的性質，所以不一定能夠真的積分出來，我們只能採樣有限個點，接著做離散求和，而這就是離散傅立葉變換

$$
X_m = \sum_{k = 0}^{n-1} x_k \cdot e^{i\frac{-2\pi}{n} mk}
$$

相當於是把連續的積分換成離散的求和，且因為預設整個採樣為整數個週期，所以旋轉時指數部分有一個 $n$ 在分母。除此之外，若採樣時不是採樣整數個週期，那麼在計算時便會發生頻譜洩漏的問題，使得原本結果應為 $0$ 的地方不為 $0$ 


## Cooley-Tukey 算法

有了 DFT 後，我們便能夠對現實世界的資料進行處理了，但還有個問題，複雜度太高了，上述的 DFT 時間複雜度為 $O(n^2)$ 。但經過適當的優化後時間複雜度能降到 $O(n\log n)$
觀察原式，我們可以把 $k$ 的奇偶分開，接著對 $e^{i\frac{-2\pi}{n}mk}$ 做變換

$$
X_m = \sum_{k = 0}^{\frac{n}{2}-1} x_{2k} \cdot e^{i\frac{-2\pi}{n} m \cdot 2k} + \sum_{k = 0}^{\frac{n}{2} - 1} x_{2k + 1} \cdot e^{i\frac{-2\pi}{n} m \cdot (2k + 1)}
$$

$$
\begin{aligned} 
e^{i\frac{-2\pi}{n} m \cdot 2k} &= e^{i\frac{-2\pi}{\frac{n}{2}}mk} \\
e^{i\frac{-2\pi}{n} m \cdot (2k + 1)} &= e^{i\frac{-2\pi}{\frac{n}{2}}mk} \cdot e^{i\frac{-2\pi}{n}m}
\end{aligned}
$$

提出常數項，並將原來的奇偶項用符號表示（方便後續閱讀）

$$
X_m = \sum_{k = 0}^{\frac{n}{2}-1} x_{2k} \cdot e^{i\frac{-2\pi}{\frac{n}{2}}mk} + e^{i\frac{-2\pi}{n}m} \cdot \sum_{k = 0}^{\frac{n}{2} - 1} x_{2k + 1} \cdot e^{i\frac{-2\pi}{\frac{n}{2}}mk} \\
X_m = E_m + e^{i\frac{-2\pi}{n}m} \cdot O_m
$$


不難發現 $O_m, E_m$ 的週期為 $\frac{n}{2}$，因為

$$
\begin{aligned}
E_{m + \frac{n}{2}} &= \sum_{k = 0}^{\frac{n}{2} - 1} x_{2k} \cdot e^{i\frac{-2\pi}{\frac{n}{2}}(m + \frac{n}{2})k} \\
&= \sum_{k = 0}^{\frac{n}{2} - 1} x_{2k} \cdot (e^{i\frac{-2\pi}{\frac{n}{2}}mk} \cdot e^{i\frac{-2\pi}{\frac{n}{2}}\frac{n}{2}k}) \\
&= \sum_{k = 0}^{\frac{n}{2} - 1} x_{2k} \cdot (e^{i\frac{-2\pi}{\frac{n}{2}}mk} \cdot e^{-2i\pi k}) \, ,k \in \mathbb{Z} \implies e^{-2i\pi k} =1 \\
&= \sum_{k = 0}^{\frac{n}{2} - 1} x_{2k} \cdot (e^{i\frac{-2\pi}{\frac{n}{2}}mk} \cdot 1) \\
E_{m + \frac{n}{2}} &= E_m
\end{aligned}
$$

接著推導 $O_m$ 外面那一項經過 $\frac{n}{2}$ 後的樣子

$$
\begin{aligned}
e^{i\frac{-2\pi}{n}(m+\frac{n}{2})} &= e^{i\frac{-2\pi}{n}m}\cdot e^{-i\pi} \\
&= e^{i\frac{-2\pi}{n}m} \cdot -1 \\
&= -e^{i\frac{-2\pi}{n}m}
\end{aligned}
$$

全部整理後，我們可以得到

$$
\begin{cases}
X_m &= E_m + (e^{i\frac{-2\pi}{n}m} \cdot O_m) \\
X_{m + \frac{n}{2}} &= E_m - (e^{i\frac{-2\pi}{n}m} \cdot O_m)
\end{cases}
$$

那麼我們便可以在計算 $X_m$ 時同時計算 $X_{m + \frac{n}{2}}$，與此同時，在計算 $O_m, E_m$ 時因為它們本身就是輸入為 $\frac{n}{2}$ 的 DFT ，所以可以遞迴處理進一步減少計算量。但這代表一開始的 $n$ 必須要是 $2$ 的冪，否則無法遞迴完全，時間複雜度可能會退化（部份情況下有其它方法，或是也可直接補 $0$ 直到長度達標）
在長度為 $2$ 的冪的情況下複雜度會是 $O(n\log n)$


## NTT

現在考慮一個情況，兩個多項式相乘，時間複雜度為 $O(n^2)$，但實際上可以透過傅立葉變換來讓複雜度降到 $O(n\log n)$，具體過程如下。對於一個 $q-1$ 次多項式，我們只要 $q$ 個點就能唯一地確定它。但即使如此，我們也不能隨意帶數值進去，否則在次方的增加下，數值會暴增。同時，我們還至少需要 $2q-1$ 個點（兩個 $q-1$ 次相乘會得到 $2q-2$ 次）。而有限體完美符合我們的需求，由於模 $p$ 導致結果必定只在 $0 \sim p-1$ 之間，同時它又是可逆的，只要在計算時補 $0$ 到長度為不小於 $2q-1$ 的 $2^s$ 就行（長度為 $2$ 的冪方便優化）。至於複數，則是會有浮點數誤差，實際使用可能會出現預期外的結果

NTT的公式

$$
X_m = \sum_{k = 0}^{n - 1} x_k \cdot g^{\frac{p - 1}{n}mk} \pmod p
$$

$n$ 為前面提到的不小於 $2q-1$ 的 $2^s$，也就是補零後的長度

$p$ 為質數，多數情況下會使用形式為 $c \cdot 2^k + 1$ 的質數，這樣能讓 $p-1$ 有更多的 $2$ 因子，使得蝶形運算更容易達成使用條件和被執行

$g$ 是模 $p$ 下的原根，且 $n \ | \ p - 1$

以下用 $\omega$ 代稱 $g^{\frac{p - 1}{n}}$

$\omega$ 是模 $p$ 下的 $n$ 次單位根，它的定義是 $1 \equiv \omega^n \pmod p \ \wedge \ \forall k \ \in \mathbb{Z}, \ 1 \le k \lt n \ \text{s.t.} \ 1 \not\equiv \omega^k \pmod p$，換句話說，它的階數在模 $p$ 下為 $n$，這能夠讓它和前面的 $e^{i\frac{-2\pi}{n}}$ 有著一樣的旋轉特性。除此之外，我們實際在計算時代入原多項式的數值正是 $\omega, \omega^2, \omega^3 ... \omega^n$，算出所有的 $X_m$ 後，再去和另一個多項式的 $X_m$ 相乘

### INTT

算完之後，反向把 $X_m$ 變成 $x_k$ 的公式如下

$$
x_k = n^{-1} \sum_{m = 0}^{n - 1} X_m \cdot g^{-\frac{p - 1}{n}mk} \pmod p
$$

$n^{-1}$ 是模 $p$ 下 $n$ 的模逆元，也就是 $n \cdot n^{-1} \equiv 1 \pmod p$，可以透過費馬小定理尋找

NTT 也可以使用上述的 Cooley-Tukey 算法優化，因為它滿足 $\omega^n \equiv 1 \pmod p \wedge \omega^{\frac{n}{2}} \equiv -1 \pmod p$，後者推導如下

由於 $\omega$ 的定義， $\omega^{\frac{n}{2}}$ 不可能在模 $p$ 下同餘 $1$

$$
\begin{aligned}
\omega^n &\equiv 1 \pmod p \\
\omega^n - 1 &\equiv 0 \pmod p \\
(\omega^{\frac{n}{2}} - 1)(\omega^{\frac{n}{2}} + 1) &\equiv 0 \pmod p \\
\omega^{\frac{n}{2}} &\equiv -1 \pmod p
\end{aligned}
$$

因為 $p$ 是質數，兩個因子相乘同餘 $0$ 時，至少有一個同餘 $0$。由 $\omega$ 的定義 $\omega^{\frac{n}{2}} - 1 \not\equiv 0$ ，所以 $\omega^{\frac{n}{2}} + 1 \equiv 0$，即 $\omega^{\frac{n}{2}} \equiv -1$


## essence of fourier transform

傅立葉變換的本質實際上就是把卷積轉換成點對點相乘

從抽象代數的層面看，這些操作就是在向量空間中對一組正交基底做點積，這些正交基底就是 DFT 裡的 $e^{i\frac{-2\pi}{n}mk}$ 、 NTT 裡的 $g^{\frac{p - 1}{n}mk}$，或是我們也可以稱它們為旋轉因子，因為它們共通點是次方的週期性。而點積能夠算出目標向量在各個基底向量上的分量，又因為基底是正交的，所以結果能有很多良好的性質（詳細資訊見下文）

如果直接從代數式推導，也可以得出結果，卷積的式子如下

$$
C(x) = \sum_k \left(\sum_j a_j \cdot b_{k-j} \right) x^k
$$

代入旋轉因子 $\omega$

$$
C(\omega) = \sum_k \left( \sum_j a_j \cdot b_{k-j} \right) \omega^k
$$

可以對 $\omega^k$ 進行一些等價變換

$$
\begin{aligned}
\omega^k &= \omega^{j + (k - j)} \\
&= \omega^j \cdot \omega^{k-j}
\end{aligned}
$$

將變換後的 $\omega$ 帶進 $\sum$ 內

$$
C(\omega) = \sum_k \sum_j (a_j \cdot \omega^j) \cdot (b_{k-j} \cdot \omega^{k-j})
$$

令 $l = k - j$

$$
\begin{aligned}
C(\omega) &= \sum_j \sum_l (a_j \cdot \omega^j) \cdot (b_l \cdot \omega^l) \\
C(\omega) &= \left( \sum_j a_j \cdot \omega^j \right) \times \left( \sum_l b_l \cdot \omega^l \right) \\
C(\omega) &= A(\omega) \cdot B(\omega)
\end{aligned}
$$


上面提到的正交的良好性質實際上是籠統的說法，以下將旋轉因子視為一個函數 $f(x)$（ $e^{i\frac{-2\pi}{n}mk} = f(mk)$ ）， $f(x)$ 必須要具有以下兩個性質才能套進傅立葉變換

1. $f(x + y) = f(x)f(y)$ / 加法群到乘法群的同態映射
2. $f(x + N) = f(x)$ / 定義在緊緻/有限群上

不難看出第一個是指數函數的充要條件

而第二個依然不難看出這是週期性，有了週期性和同態映射，我們就能推導單位模長和正交，同時還能保證不會發散

單位模長：

$$
\begin{aligned}
f(x)^N &= f(Nx) \\
&= f(0) \\
&= 1
\end{aligned}
$$

$$
f(x)^N = 1 \implies f(x) \text{為 N 次單位根} \implies |f(x)| = 1
$$

而舒爾引理保證兩個不同的不可約表示，它們的內積必然為 $0$，也就是正交
