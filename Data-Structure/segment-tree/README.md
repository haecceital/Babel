# segment-tree

線段樹，能處理一些像是區間和、區間最大/小之類的問題
<br>
實際上只要資料和對應的運算構成一個半群就能儲存在線段樹上，也就是滿足封閉性和結合律

封閉性是指運算結果不會超出資料的範圍，像是整數 + 整數不會出現分數一樣
<br>
結合律則是運算的順序不會影響結果，如： `A + B + C = A + (B + C)`

舉個最簡單的例子，正整數和加法能構成一個半群 $(\mathbb{N}^+, +)$

雖然理論上只要能構成半群就能被線段樹儲存，但是多數情況下(包含我寫的 [segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)) 都額外需要一個單位元素，而半群 + 單位元素則會構成么半群。原因是在 query 時我們可能會進到查詢範圍外的區間，如果有單位元素就可以直接回傳單位元素且不影響計算結果。或是我們也可以直接在 query 前判斷該區間是否在我們要的區間內，但是那會讓增加更多的判斷，相比之下直接回傳單位元素更簡潔

單位元素相當於加法中的 $0$ 、乘法中的 $1$ ，任何元素和他運算結果依然是自己

另外，[segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)裡的 ApplyLazy 會接受三個值，分別是：舊值、操作、區間長度

這裡的操作也是一個么半群，他也有自己的二元運算，並且這個么半群跟資料的么半群不保證一樣

以下我們假設：

1. 資料的二元運算([segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)中的 merge)是 $f(x, \ y)$
2. 操作的二元運算是 $g(x, \ y)$
3. apply_lazy 是 $h(v_{old}, \ v, \ len)$

而 apply_lazy 必須要滿足以下條件：

$$
\begin{aligned}
& 1. \ h(h(v_{old}, \ v_1, \ L), \ v_2, \ L) = h(v_{old}, \ g(v_1, \ v_2), \ L) \\
& 2. \ h(f(v_{old_{1}}, \ v_{old_{2}}), \ v, \ L_1 + L_2) = f(h(v_{old_{1}}, \ v, \ L_1), \ h(v_{old_{2}}, \ v, \ L_2))
\end{aligned}
$$

可以理解為 apply_lazy 會收到舊值、操作、這個舊值涵蓋的區間，然後算出這個區間新的值

而 apply_lazy 就是么半群作用

如果太抽象的話可以看以下的例子


### 一些例子

以下的例子可能無法用我的[segment-tree.cpp](/Data-Structure/segment-tree/segment-tree.cpp)實現，因為我有些地方實作有問題(我沒有實作操作的 merge，而是直接拿資料的 merge 去用)

一個儲存區間和的線段樹，操作則是區間賦值

merge 就是加法 $mergeData(x, \ y) = x + y$ <br>
操作的二元運算是 $mergeAction(x, \ y) = y$ <br>
apply_lazy 則是 $applyLazy(v_{old}, \ v, \ len) = v * len$

要注意的是這裡的 $v$ 是操作，他不一定是一些具體的數值，也可以是一個函數

例如： $v(v_{old}) = 1$  (賦值為 $1$ ) <br>
當然多數情況下都會是一個值，因為通常操作不會那麼複雜。我的例子也都把操作當成一般的值去運算

假設我現在要把 $1 \sim 4$ 的區間全部賦值為 $1$ ，那麼我會先找到代表 $1 \sim 4$ 區間和的節點 $x$ ，接著 $x = applyLazy(x, \ 1, \ 4)$ ，之後 $x$ 的值會變成 $4$ ，因為這是 $1 \sim 4$ 新的區間和


如果我把操作改成區間加法 <br>
merge_data 不變 $mergeData(x, \ y) = x + y$ <br>
merge_action 會是 $mergeAction(x, \ y) = x + y$ <br>
而 apply_lazy 會變成 $applyLazy(v_{old}, \ v, \ len) = v_{old} + v * len$
