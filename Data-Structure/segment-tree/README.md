# segment-tree

線段樹，之前打競程時沒有深入理解這個資料結構到底能存什麼，只知道能處理一些像是區間和之類的問題
<br>
後來研究了一下發現只要資料跟運算滿足半群的關係就能被線段樹儲存，也就是只要滿足封閉性和結合律

結合律指的是運算的順序不會影響結果，如： `A + B + C = A + (B + C)`
<br>
封閉性則是運算結果不會超出資料的範圍，像是整數 + 整數不會出現分數一樣

雖然理論上只要能構成半群就能被線段樹儲存，但是多數情況下(包含我寫的 [segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp))都額外需要一個單位元素，原因是在 query 時我們可能會進到範圍外的區間，如果有單位元素的話我們就可以直接回傳單位元素。或是我們也可以直接在 query 前判斷該區間是否在我們要的區間內，但是那會讓增加更多的判斷，相比之下直接回傳單位元素更簡潔

單位元素相當於加法中的 0、乘法中的 1，任何元素和他運算結果依然是自己

另外，[segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)裡的 ApplyLazy 會接受三個值，分別是：舊值、操作、區間長度

首先，這裡的操作也是一個么半群，也就是說他也有自己的二元運算，並且這個么半群跟資料的么半群不一定一樣

以下我們假設 <br>
資料的二元運算([segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)中的 merge)是 $$ f(x, y) $$ <br>
操作的二元運算是 $$ g(x, y) $$ <br>
apply_lazy 是 $$ h(old\_v, v, len) $$

而 apply_lazy 必須要滿足以下條件：
1. $$ h(h(old\_v, v_1, L), v_2, L) = h(old\_v, g(v_1, v_2), L) $$
2. $$ h(f(old\_v_1, old\_v_2), v, L_1 + L_2) = f(h(old\_v_1, v, L_1), h(old\_v_2, v, L_2)) $$

可以理解為 apply_lazy 會收到舊值、操作、這個舊值涵蓋的區間，然後算出這個區間新的值

而 apply_lazy 就是么半群作用

如果太抽象的話可以看以下的例子


## 一些例子

以下的例子可能無法用我的[segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)實現，因為我有些地方實作有問題(我沒有實作操作的 merge)

一個儲存區間和的線段樹，操作則是區間賦值

merge 就是加法 <br>
操作的二元運算是 $$ merge\_action(x, y) = y $$ <br>
apply_lazy 則是 $$ apply\_lazy(old, v, len) = v * len $$

要注意的是這裡的 $$ v $$ 是操作，他不一定是一些具體的數值，也可以是一個函數 <br>
例如：$$ v(old) = 1 $$ (相當於是賦值成 1) <br>
當然多數情況下都會是一個值，因為通常操作不會那麼複雜。我的例子也都把操作預設為一般的值

假設我現在要把 1 ~ 4 的區間全部賦值為 1 ，那麼我會先找到代表 1 ~ 4 區間和的節點 x，然後直接 $$ x = apply\_lazy(x, 1, 4) $$，這時候 x 的值會變成 4，因為這是 1 ~ 4 新的區間和


如果我把操作改成區間加法 <br>
merge_action 會是 $$ merge\_action(x, y) = x + y $$ <br>
而 apply_lazy 會變成 $$ apply_lazy(old\_v, v, len) = old\_v + v * len $$
