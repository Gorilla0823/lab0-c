# 2020q1 Homework1 (lab0)
contributed by < `Gorilla0823` >

## 實驗環境
```shell
$ uname -a
Linux 5.3.0-28-generic Linux ubuntu 4.15.0-51-generic #55-Ubuntu SMP Wed May 15 14:27:21 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux


$ gcc --version
gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
```


## 作業要求
- 詳細閱讀 [C Programming Lab](http://www.cs.cmu.edu/~213/labs/cprogramminglab.pdf) ，依據指示著手修改 `queue.[ch]` 和連帶的檔案，測試後用 Git 管理各項修改，記得也該實作 `q_sort` 函式
- 修改排序所用的比較函式，變更為 `natural sort`，在 “simulation” 也該做對應的修改，得以反映出 `natural sort` 的使用。
- 運用 Valgrind 排除 qtest 實作的記憶體錯誤，並透過 Massif 視覺化 “simulation” 過程中的記憶體使用量，需要設計對應的實驗
- 研讀論文 Dude, is my code constant time?，解釋本程式的 “simulation” 模式是如何透過以實驗而非理論分析，達到驗證時間複雜度，需要解釋 Student’s t-distribution 及程式實作的原理;
- 解釋 select 系統呼叫在本程式的使用方式，並分析 console.c 的實作，說明其中運用 CS:APP RIO 套件 的原理和考量點。可對照參考 CS:APP 第 10 章重點提示 :arrow_forward: 為避免「舉燭」，請比照 qtest 實作，撰寫獨立的終端機命令解譯器 (可建立新的 GitHub repository)

## 開發過程

### queue.h


```cpp=
typedef struct {
    list_ele_t *head; 
    list_ele_t *tail;
    size_t size;
} queue_t;
```

根據作業要求，增加了 `size` 來記錄 queue 的大小以及 `tail` 指向 queue 中的最後一個元素。


### queue.c

#### q_new

- 建立一個空的 queue
- 當 `malloc` 失敗時需要回傳 `NULL`

```cpp=
queue_t q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    
    if (!q) {
        return NULL;
    }

    q->tail = NULL;
    q->head = NULL;
    q->size = 0;
    return q;
}
```

1. 將 `tail` 及 `head` 指向 `NULL`
2. 將 queue `size` 設為 `0` 

#### q_size

- 回傳 queue 的元素數量
- queue 為 `NULL` 時 return `0`

```cpp=
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}
```

#### q_free

- 釋放 queue 所占用的記憶體
- 當 queue 為 `NULL` 時，直接回傳 

```cpp=
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }

    list_ele_t *tmp = NULL;
    while (q->head) {
        tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    q->tail = NULL;
    q->head = NULL;
    q->size = 0;
    free(q);
}
```

1. 設立 1 個指向 `NULL` 的 `list_ele_t` 結構，從 `head` 刪到 `tail` ，並且 `free` 掉每個 Node 及其包含的字串。
2. 將 `tail` 及 `head` 指向 `NULL` ，並且將 queue  的 `size` 歸 `0` 。

#### q_insert_head
- 從 queue 的 `head` 插入一個元素
- 當 queue 為 NULL 或 `malloc` 失敗時 回傳 false
- `*s` 指向要被插入的元素

```cpp=
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    size_t length = strlen(s);
    char *str = malloc(length + 1);  
    if (newh && str) {
        strncpy(str, s, length);
        str[length] = '\0';
        newh->value = str;
        newh->next = q->head;
        q->head = newh;
        if (!q->tail) {
            q->tail = newh;
        }
        q->size++;
        return true;
    } else {
        free(newh);  
        free(str);  
        return false;
    }
}
```
1. 這個部分修改了很多次，原本採用 `strcpy` 來複製欲新增的元素，因為 compiler 警告這種行為會造成 buffer overrun ，所以改為 `strncpy` 。
2. 剛開始以為 `malloc` 失敗只會回傳 `NULL` pointer， 查了資料後發現也會回傳 `non-null` pointer ， 所以改用 `if-else`來回收記憶體。
 
 經過查詢[Linux man page](https://linux.die.net/man/3/malloc)後得知其對於`malloc` 回傳值的敘述如下：
 > The malloc() function allocates size bytes and returns a pointer to the allocated memory. The memory is not initialized. If size is 0, then malloc() returns either NULL, or a unique pointer value that can later be successfully passed to free().



#### q_insert_tail
- 從 queue 的 `tail` 新增元素
- 如果成功新增則回傳 `true`
- 若 queue 為 `NULL` 或分配記憶體失敗時則回傳 `false`
- `*s` 指向要被插入的元素

```cpp=
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q || !q->head) {
        return false;
    }
    list_ele_t *tmp = q->head;
    if (sp) {
        memset(sp, 0, bufsize);
        strncpy(sp, tmp->value, bufsize - 1);
    }
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}
```
此 function 與 `insert_head` 類似。

#### q_remove_head

- 將 queue 從 `head` 刪除
- 如果成功刪除則回傳 `true`
- 當 queue 為空或為 `NULL` 時回傳 `false`
- 若 `sp` 是 `non-NULL` 且一個元素被刪除時，複製被刪除的元素到 `*sp` 中。( 上限為 `bufsize-1`並且 `*sp` 的最後一位須為 null terminator。 )
- 被 list 元素使用的記憶體及其字串需要被 `free`。 

```cpp=
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }
    list_ele_t *tmp = q->head;
    if (sp) {
        memset(sp, 0, bufsize);
        strncpy(sp, tmp->value, bufsize - 1);
    }
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}
```

#### q_reverse
- 將 queue 的元素反轉
- queue 為 `NULL` 或 `empty` 時直接回傳
- 不可分配新的記憶體位址和 `free` 原有的記憶體位址

``` cpp=
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    list_ele_t *curr = q->head, *prev = NULL, *next = NULL;
    q->tail = q->head;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
}
```
1. 設定 `1` 個分別指向 `head` 的 `*curr` 及 `2` 個指向 `NULL` 的 `prev` 和 `next`，並將 `head` 指向 `tail` 。
2. 接著從 `head` 走到 `NULL` ，過程中運用 pointer 將 queue 轉向。
3. 最後將 `head` 指向 `*prev` 。

#### q_sort
- 將 queue 中元素小到大做排序
- 如果 queue 是 `NULL` 或 `empty`，直接回傳。
- 如果 queue 裡面只有 `1` 個元素，不做任何動作。
- 使用 natural sort ( 額外新增 ) 

``` cpp=
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }
    q->head = mergeSortList(q->head);
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}
```
1. 將 `mergeSortList` function sort 的結果傳給 `head`。
2. 移動至 `tail` 並設立 `tail` 。

#### 
``` cpp=
#include "natsort/strnatcmp.c"

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    // merge with recursive
    if (!l2)
        return l1;
    if (!l1)
        return l2;
    if (strnatcmp(l1->value, l2->value) < 0) {
        l1->next = merge(l1->next, l2);
        return l1;
    } else {
        l2->next = merge(l1, l2->next);
        return l2;
    }
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    // merge sort
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    // split list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    // sort each list
    // for()
    list_ele_t *l1 = mergeSortList(head);
    list_ele_t *l2 = mergeSortList(fast);
    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}
```
採用 merge sort 進行排序，即使在 worst case ， merge sort 也是 $O(nlogn)$ 的時間複雜度。
實作的部份參考 [Linked List Sort](https://npes87184.github.io/2015-09-12-linkedListSort/)



## 參考資料
* [2020 春季 lab0c 作業說明](https://hackmd.io/@sysprog/linux2020-lab0) 
* [C Programming Lab: Assessing Your C Programming Skills](http://www.cs.cmu.edu/~213/labs/cprogramminglab.pdf)
