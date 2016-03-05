// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
#define NOMINMAX


// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#pragma once

// Standard Library includes
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

// Boost includes
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>

// Boolinq include
#include <boolinq/boolinq.h>