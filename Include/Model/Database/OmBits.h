#pragma once

//位操作辅助宏定义 测试某位是否为1
#define TEST_BIT_64(number,index) (0!=((1ull<<(index)) & (number)))

//将某位设置为1
#define SET_BIT_64(number,index) (  (number) |=(1ull<<(index)) )

//将某位设置为0
#define UNSET_BIT_64(number,index) (  (number) &=  ( ~(1ull<<(index)) )  )


// 获取某段的值。[higher bits|target bits|lower bits]
#define PARSE_BITS_VALUE_64(number, nHigherBits, nLowerBits) ((static_cast<UInt64>(number)) << (nHigherBits) >> ((nHigherBits) + (nLowerBits)))

// 将两段组合。 [higher bits|lower bits]
#define COMBINE_BITS_VALUE_64(higherBits, nLowerBits, lowerBits) ((static_cast<UInt64>(higherBits)) << (nLowerBits) | (lowerBits))
