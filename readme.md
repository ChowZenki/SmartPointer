﻿輕量級基於引用計數的智能指針(Smart Pointer)
==========================

_Ralph Shane_ (free2000fly at gmail dot com)

[Smart Pointer](https://github.com/free2000fly/SmartPointer)


概述
==========================

本實現包含兩种指針: 強指針 strong_ptr 和 弱指針 weak_ptr。基本上可以替換 std::shared_ptr 和 std::weak_ptr.

強指針對象持有物件的指針並增加“強”引用計數；當強指針對象析搆時，“強”引用計數自減；當“強”引用計數自減到 0 時，強指針對象釋放物件實體。

弱指針對象不負責管理所持有物件的生命周期, 它僅僅維護著一個“弱”引用計數, 並在需要時從自身生成一個強指針. 弱指針的存在是爲了避免因循環引用 (circular references) 而導致智能指針持有的物件無法釋放的情況出現。

本實現不是“多綫程安全”的，用戶必須自己處理多綫程環境的各種加鎖和解鎖工作。

實現細節
==========================

1. 簡單的類 `ref_count` 用於實現對“強”引用計數和“弱”引用計數的操作。

2.  基類 `base_ptr` 實現了強指針和弱指針的絕大部分邏輯，這個類是強指針和弱指針共同的基類。有兩個成員變量，`ref_count` 對象實體指針 `m_counter` 和 raw 物件指針。這個類的關鍵點有四: 

    (1) 在非零的 raw 物件指針傳入到構造函數時，持有該指針，並創建 `ref_count` 對象實體指針 `m_counter` 成員變量，此時“強”引用計數為 1，而“弱”引用計數為 0。

    (2) 在“拷貝構造函數”的參數裏傳入強指針或弱指針對象時，調用 `acquire` 函數。

    (3) `acquire` 函數裏完成兩件事: 持有傳入的 `ref_count` 對象指針，增加“強”引用計數或“弱”引用計數；持有傳入的 raw 物件指針。

    (4) 在 `base_ptr` 對象析搆時，調用最關鍵的 `release` 函數。`release` 函數針對自身 `base_ptr` 對象是強指針還是弱指針決定“強”引用計數或“弱”引用計數的自減。當“強”引用計數為 0 時，釋放（delete）持有的物件。繼續下一步的判斷，當“強”引用計數和“弱”引用計數都為 0 時，釋放（delete）`ref_count` 對象實體指針 `m_counter`。然後將 raw 物件指針 `m_ptr` 和 `m_counter` 變量歸零。

3.  `strong_ptr` 類基本上就是轉發 `base_ptr` 基類的操作。`weak_ptr` 類與 `strong_ptr` 類似，主要不同點就是將對 raw 物件指針的直接操作屏蔽掉。


關於 `make_strong_ptr` 函數
==========================

除了 vs2010/vs2012 外，其它編譯器不支持形如

    template <typename T, typename A1>
    strong_ptr<T> make_strong_ptr(A1 &&a1)
    {
        return strong_ptr<T> ( new T(a1) );
    }

的語法，被迫改成形如以下的樣子，這樣一來函數 `make_strong_ptr` 就不再支持常數形式的傳入參數

    template <typename T, typename A1>
    strong_ptr<T> make_strong_ptr(A1 const &a1)
    {
        return strong_ptr<T> ( new T(a1) );
    }


測試平臺
==========================

通過 

    Visual Studio 2003/2005/2008/2010/2012

未通過
    
    Visual Studio 6

