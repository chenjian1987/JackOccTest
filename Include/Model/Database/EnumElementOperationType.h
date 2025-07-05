#pragma once
#include "JDevService.h"


namespace JackC
{
    /// <summary>
    /// IElementManager内部操作类型
    /// </summary>
    enum class EnumElementOperationType : int32_t
    {
        Undo,               ///< Undo 操作
        Redo,               ///< Redo 操作
        RollBack,           ///< RollBack操作
        RestoreFromUndo,    ///< 从Undo数据中恢复
    };
}

