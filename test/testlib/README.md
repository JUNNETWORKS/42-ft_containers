# GoogleTest互換テストライブラリ

## これはなに？

GoogleTestのテストコードが基本そのまま動く自作テストライブラリです。

## まえがき

42Tokyoの課題ではC++98を使ってコードを書く必要があるのですが、GoogleTestはC++11以上を要求しているので使えません。

しかし、GoogleTestは便利ですし、有名なテスティングフレームワークを触るのも良い勉強なので、GoogleTestを使うのですが、課題提出時にはそれらのテストはC++11を要求するのでC++98に幽閉された我々は提出時には使えないのです。

C++98 の世界でGoogleTestに憧れを抱く我々に舞い降りた救世主こそこの自作テストライブラリなのです。

### 使い方

GoogleTestを利用しているコードにて `#include <gtest/gtest.h>` を `#include "testlib/testlib.hpp"` に置き換えてください。

もしくは以下のようにバージョンによってGoogleTestと切り替えるようにしても良いでしょう。

```c++
#if __cplusplus >= 201103L
#include <gtest/gtest.h>
#else
#include "testlib/testlib.hpp"
#endif
```

そしてGoogleTestを利用しているcppファイルを `testlib_main.cpp` 内で include してください。

例: `vector_test.cpp` と `stack_test.cpp` 内でGoogleTestを利用している場合、以下のように `testlib_main.cpp` 内に include する。

```c++
/***** Include all the files that use GoogleTest to test *****/

#include "stack_test.cpp"
#include "vector_test.cpp"
```

そして以下のようにコンパイルする。(各自必要なオプションなどは付け足してください)

```bash
clang++ --std=c++98 test/testlib/testlib_main.cpp
```

## 注意

基本的には以下に載せる自分に必要なアサート類しか実装していません。

- `EXPECT_EQ`
- `EXPECT_NE`
- `EXPECT_TRUE`
- `EXPECT_FALSE`
- `EXPECT_THROW`
- `ASSERT_EQ`
- `ASSERT_NE`
- `ASSERT_TRUE`
- `ASSERT_FALSE`

もしこれ以外のアサート類が必要な場合は自分で `testlib.hpp` に実装してください。

また、このテストライブラリは「GoogleTestで使ってたテストコードがC++98で動けば良いや」という思想で作られたものです。

なので基本的には GoogleTest のような便利な機能は殆ど無く、1つでもKOが出た瞬間そこでプログラムは `assert()` によって終了します。

もし KO になってもプログラムを実行し続けたかったり、プログラムの最後に全体の実行結果などを表示したい場合は例によって自分で実装してください。
