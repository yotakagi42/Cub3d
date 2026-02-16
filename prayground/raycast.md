# raycast 関数の解説

DDA（Digital Differential Analyzer）アルゴリズムを使ったレイキャスティング。
画面の各縦ライン（x=0〜639）ごとにレイを飛ばし、壁を見つけてテクスチャ付きで描画する。

## 処理の全体フロー

```
raycasting(): x=0〜SCREEN_W をループ
  ① init_ray       → レイの方向とグリッド移動コストを計算
  ② calc_step      → 進む方向と最初のグリッド線までの距離を算出
  ③ perform_dda    → グリッドを1マスずつ進めて壁を探す
  ④ calc_wall_dist → カメラ平面への垂直距離から壁の高さを計算
  ⑤ テクスチャ選択  → 壁の面の方角でテクスチャを選択
  ⑥ draw_wall_stripe → テクスチャから色を取って1列分を描画
```

---

## ① init_ray（L26-42）— レイの初期化

```c
ray->camera_x = 2 * x / (double)SCREEN_W - 1;
```
- `camera_x`：画面上のx座標を **-1.0〜1.0** の範囲に変換。左端が`-1`、中央が`0`、右端が`1`。
- `plane_x/y`に対する倍率になる。
- 向いている方向の右側が正ベクトル、左側が負ベクトル。
- よって右端を`1`、左端を`-1`にしている。

```c
ray->ray_dir_x = game->dir_x + game->plane_x * ray->camera_x;
ray->ray_dir_y = game->dir_y + game->plane_y * ray->camera_x;
```
- `ray_dir_x/y`：レイの方向ベクトル。プレイヤーの向き(`dir`)にカメラ平面(`plane`)を加味して計算。
- `ray_dir`の奥行き成分は常に`dir`でしかない
- `|dir|`は常に1という前提で計算を進める
- `plane`はFOV（視野角）を決定する。

```c
ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
```
- `delta_dist_x`：**X方向に『1』進むために、レイ・ベクトル（ray_dir）を何倍すればいいか？**。
- 方向が0のときは`1e30`（十分大きい値）を設定して0除算を回避。

---

## ② calc_step（L44-66）— ステップ方向と初期サイド距離

```c
if (ray->ray_dir_x < 0)
{
    ray->step_x = -1;
    ray->side_dist_x = (game->pos_x - ray->map_x) * ray->delta_dist_x;
}
else
{
    ray->step_x = 1;
    ray->side_dist_x = (ray->map_x + 1.0 - game->pos_x) * ray->delta_dist_x;
}
```
- `step_x/y`：DDAでグリッドを進む方向（`+1` or `-1`）
- `side_dist_x/y`：プレイヤー位置から**最初のグリッド線までの距離**

---

## ③ perform_dda（L68-89）— 壁を見つけるまでグリッドを進む

```c
if (ray->side_dist_x < ray->side_dist_y)
{
    ray->side_dist_x += ray->delta_dist_x;
	// 次の判定用にside_distを更新
    ray->map_x += ray->step_x;
    ray->side = 0;  // X面（東西の壁）にヒット
}
else
{
    ray->side_dist_y += ray->delta_dist_y;
    ray->map_y += ray->step_y;
    ray->side = 1;  // Y面（南北の壁）にヒット
}
```
- **常に近い方のグリッド線に進む**のがDDAの核心。
- `side=0`：X方向の壁（東or西の面）にヒット。
- `side=1`：Y方向の壁（南or北の面）にヒット。

### ヒット判定（L84-87）

```c
if (ray->map_x < 0 || ray->map_x >= game->map_w
    || ray->map_y < 0 || ray->map_y >= game->map_h
    || game->world_map[ray->map_x][ray->map_y] > 0)
    ray->hit = 1;
```
- まず**マップ範囲外チェック**：`map_x/y` が `0` 未満、または `map_w/h` 以上なら即ヒット扱い。
  - これがないとレイがマップ外に飛び出して配列の範囲外アクセスになる。
- 範囲内なら `world_map[map_x][map_y] > 0` で**壁判定**。
  - `0` = 空間（通過可能）、`1`以上 = 壁（ヒット）。
  - 値が `1,2,3,...` と異なる場合はテクスチャの種類を変えることもできる（現在は方角で決定）。
- どちらかの条件を満たすと `hit = 1` になり、DDAのwhileループを抜ける。

---

## ④ calc_wall_dist（L91-104）— 壁の高さを計算

```c
if (ray->side == 0)
    ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
	//垂直距離＝x方向の距離/ray->dir_x
	//<=> x方向の距離 * delta_dist_x
	//<=> side_dist_x - delta_dist_x
else
    ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);
```
- **垂直距離（perp_wall_dist）** を計算。
- ユークリッド距離ではなく**カメラ平面に垂直な距離**を使うことで、**魚眼レンズ効果を防止**。

```c
ray->line_height = (int)(SCREEN_H / ray->perp_wall_dist);
```
- 壁が近いほど `perp_wall_dist` が小さく → `line_height` が大きく → 壁が大きく描画される。

```c
ray->draw_start = -ray->line_height / 2 + SCREEN_H / 2;
ray->draw_end = ray->line_height / 2 + SCREEN_H / 2;
```
- 壁を**画面の中央に上下対称**に配置。画面外にはみ出す場合はクランプ。

---

## ⑤ テクスチャ番号の決定（L149-153）

```c
ray.tex_num = 0;// 東面（←西へ進むレイがX壁にヒット）
if (ray.side == 0 && ray.ray_dir_x > 0) ray.tex_num = 1;
// 西面（→東へ進むレイがX壁にヒット）
if (ray.side == 1 && ray.ray_dir_y < 0) ray.tex_num = 2;
// 南面（↑北へ進むレイがY壁にヒット）
if (ray.side == 1 && ray.ray_dir_y > 0) ray.tex_num = 3;
// 北面（↓南へ進むレイがY壁にヒット）
```
- レイが当たった**壁の面の方角**に応じて、`textures[0〜3]`のどれを使うか選択。

---

## ⑥ draw_wall_stripe（L106-134）— テクスチャを描画

```c
if (ray->side == 0)
    wall_x = game->pos_y + ray->perp_wall_dist * ray->ray_dir_y;
else
    wall_x = game->pos_x + ray->perp_wall_dist * ray->ray_dir_x;
wall_x -= floor(wall_x);
```
- `wall_x`：壁のどの位置にヒットしたかの**小数部分**（0.0〜1.0）。これがテクスチャのX座標になる。

```c
ray->tex_x = (int)(wall_x * (double)TEX_W);
```
- テクスチャのピクセルX座標に変換（0〜63）。

```c
if ((ray->side == 0 && ray->ray_dir_x > 0)
    || (ray->side == 1 && ray->ray_dir_y < 0))
    ray->tex_x = TEX_W - ray->tex_x - 1;
```
- レイの方向によってテクスチャを**左右反転**。壁を裏側から見たときにテクスチャが鏡像にならないようにする。

```c
step = 1.0 * TEX_H / ray->line_height;
tex_pos = (ray->draw_start - SCREEN_H / 2 + ray->line_height / 2) * step;
```
- `step`：画面の1ピクセルあたりのテクスチャの進行量。
- 壁が画面より大きいとき、テクスチャの途中から描画が始まるので `tex_pos` でオフセットを計算。

```c
color = game->textures[ray->tex_num].addr[TEX_H * ((int)tex_pos & (TEX_H - 1)) + ray->tex_x];
```
- `(int)tex_pos & (TEX_H - 1)`：テクスチャY座標のラップアラウンド（64x64なので `& 63`）。
- `TEX_H`が`2^n`の時しかビット演算でのラップアラウンドはできない
- テクスチャの1次元配列から直接ピクセル色を取得。
