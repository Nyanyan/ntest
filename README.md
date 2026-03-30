# NTest
Ntest is a strong othello program. It can be used in command-line mode or using the graphical viewer NBoard.

Requirements:

* 64-bit Windows, Mac, or Linux
* popcnt instruction  
This is available on most Intel CPUs since 2009 and AMD systems since 2008. You will get an error message if trying to run on a computer without popcnt.

32-bit NTest is no longer maintained but it is available as the very first commit in this repository: a0949a58df442b1d7f65b70156e22758bdf5fdca . On a 64-bit machine it goes about 2/3 as fast as 64-bit NTest.
It can also be used on machines without popcnt.

## オリジナルからの変更点

このリポジトリでは、オリジナルの NTest に加えて以下の拡張を入れています。

### GTP 対応

GTP モードを追加しています。

* 起動方法: `ntest --gtp`
* GTP ハンドラは `RunGtp()` を通して動作します

### `-l` オプション

edax 風のレベル指定を追加しています。

* 起動方法: `ntest -l 10`
* `l` をレベルとすると
* 中盤は `l` 手を 100% 読みます
* 終盤は残り `2l` マス以下で完全読みに移行します

例:

* `ntest -l 10`
* `ntest -l 10 o`

これは従来の `sL` ベースの NTest レベル定義とは別物です。

### `-t` オプション

使用スレッド数の上限指定を追加しています。

* 起動方法: `ntest -t 8`
* `-l` と併用可能です: `ntest -t 8 -l 10`

注意:

* 現在の探索本体は並列探索ではなく、基本的にシングルスレッドです
* そのため `-t` は将来の拡張を見据えた上限指定で、現状で効く主な対象は外部ビューア用の補助入力スレッドです
* `-t 1` を指定すると、その補助スレッドも作らず、実質 1 スレッド動作になります

## Other Resources

From the Wayback Machine, [NTest's old homepage](https://web.archive.org/web/20131011003457/http://othellogateway.com/ntest/Ntest/)
