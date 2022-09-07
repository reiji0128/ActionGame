#include "Collision.h"
#include <algorithm>
#include <array>

//------------------------------------------------//
//                    球体                        //
//------------------------------------------------//

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="center">中心座標</param>
/// <param name="radius">半径</param>
Sphere::Sphere(const Vector3& center, float radius)
    :mCenter(center)
    ,mRadius(radius)
{
}

/// <summary>
/// 球体内に点が含まれるか？
/// </summary>
/// <param name="point">点の座標</param>
/// <returns>
/// true  : 含まれている
/// false : 含まれていない
/// </returns>
bool Sphere::Contains(const Vector3& point) const
{
    //球の中心と点との距離の2乗を計算したものと半径の2乗を比較
    float distSq = (mCenter - point).LengthSq();
    return distSq <= mRadius * mRadius;
}

//------------------------------------------------//
//                      線分                      //     
//------------------------------------------------//

/// <summary>
/// コンストラクタ
/// </summary>
Line::Line()
    :mLineStart(0.0f,0.0f,0.0f)
    ,mLineEnd(0.0f,0.0f,0.0f)
    ,mNormalizeDirection(0.0f,0.0f,0.0f)
    ,mLineLength(0.0f)
{
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="start">始点座標</param>
/// <param name="end">終点座標</param>
Line::Line(const Vector3& start, const Vector3& end)
    :mLineStart(start)
    ,mLineEnd(end)
    ,mNormalizeDirection(0.0f, 0.0f, 0.0f)
    ,mLineLength(0.0f)
    ,mRotatable(false)
{
    Vector3 diff;
    diff = mLineEnd - mLineStart;
    mLineLength = diff.Length();

    mNormalizeDirection = (1.0f / mLineLength) * diff;
}

//------------------------------------------------//
//   軸並行ボックス(Axis - Aligned Bounding Box)  //
//------------------------------------------------//

/// <summary>
/// コンストラクタ
/// </summary>
AABB::AABB()
{
    mMin = Vector3(0.0f, 0.0f, 0.0f);
    mMax = Vector3(0.0f, 0.0f, 0.0f);
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="min">ボックスの最小座標</param>
/// <param name="max">ボックスの最大座標</param>
AABB::AABB(const Vector3& min, const Vector3& max)
    :mMin(min)
    ,mMax(max)
{
    // ボックスの頂点を計算
    CalcVertex();
}

/// <summary>
/// ボックスの頂点（8個）を再計算する関数
/// </summary>
void AABB::CalcVertex()
{
    //ボックスの上側4点
    mVertex[0] = Vector3(mMin.x, mMin.y, mMax.z);
    mVertex[1] = Vector3(mMax.x, mMin.y, mMax.z);
    mVertex[2] = Vector3(mMin.x, mMax.y, mMax.z);
    mVertex[3] = Vector3(mMax.x, mMax.y, mMax.z);

    //ボックスの下側4点
    mVertex[4] = Vector3(mMin.x, mMin.y, mMin.z);
    mVertex[5] = Vector3(mMax.x, mMin.y, mMin.z);
    mVertex[6] = Vector3(mMin.x, mMax.y, mMin.z);
    mVertex[7] = Vector3(mMax.x, mMax.y, mMin.z);
}

/// <summary>
/// 最小・最大座標の更新処理
/// </summary>
/// <param name="point">点の座標</param>
void AABB::UpdateMinMax(const Vector3& point)
{
    //x,y,zそれぞれの最大・最小を求める
    mMin.x = Math::Min(mMin.x, point.x);
    mMin.y = Math::Min(mMin.y, point.y);
    mMin.z = Math::Min(mMin.y, point.z);

    mMax.x = Math::Max(mMax.x, point.x);
    mMax.y = Math::Max(mMax.y, point.y);
    mMax.z = Math::Max(mMax.z, point.z);

    // ボックスの頂点を計算
    CalcVertex();
}

/// <summary>
/// 点がボックス内に含まれるか?
/// </summary>
/// <param name="point">点の座標</param>
/// <returns>
/// true  : 含まれている
/// false : 含まれていない
/// </returns>
bool AABB::Contains(const Vector3& point) const
{
    bool outside = point.x < mMin.x ||
                   point.y < mMin.y ||
                   point.z < mMin.z ||
                   point.x > mMax.x ||
                   point.y > mMax.y ||
                   point.z > mMax.z;

    //もし1つもtrueがなければ、ボックスの内側に点が存在する
    return !outside;
}

/// <summary>
/// 点との最小距離を求める関数
/// </summary>
/// <param name="point">点の座標</param>
/// <returns>点との最小距離</returns>
float AABB::MinDistSq(const Vector3& point) const
{
    //それぞれの軸での差を取る
    float dx = Math::Max(mMin.x - point.x, 0.0f);
    dx = Math::Max(dx, point.x - mMax.x);
    float dy = Math::Max(mMin.y - point.y, 0.0f);
    dy = Math::Max(dy, point.y - mMax.y);
    float dz = Math::Max(mMin.z - point.z, 0.0f);
    dz = Math::Max(dz, point.z - mMax.z);

    //3次元空間での距離の2乗の公式より
    return dx * dx + dy * dy + dz * dz;
}

/// <summary>
/// 回転処理
/// </summary>
/// <param name="q">回転角度</param>
void AABB::Rotate(const Quaternion& q)
{
    //ボックスの8つの頂点の配列を作る
    std::array<Vector3, 8> points;
    //最小値は常にコーナー
    points[0] = mMin;
    //2つの最小値と1個の最大値の並べ替え
    points[1] = Vector3(mMax.x, mMin.y, mMin.z);
    points[2] = Vector3(mMin.x, mMax.y, mMin.z);
    points[3] = Vector3(mMin.x, mMin.y, mMax.z);
    // 2つの最大値と1個の最小値の並べ替え
    points[4] = Vector3(mMin.x, mMax.y, mMax.z);
    points[5] = Vector3(mMax.x, mMin.y, mMax.z);
    points[6] = Vector3(mMax.x, mMax.y, mMin.z);
    // 最大値は常にコーナー
    points[7] = Vector3(mMax);

    //最初の点を回転
    Vector3 p = Vector3::Transform(points[0], q);
    //最小値と最大値を求めるために最初の点を入れてリセット
    mMin = p;
    mMax = p;
    //回転によって最大値と最小値を求めなおす
    for (size_t i = 1; i < points.size(); i++)
    {
        p = Vector3::Transform(points[i], q);
        UpdateMinMax(p);
    }
}

/// <summary>
/// スケーリング
/// </summary>
/// <param name="x">Xの拡大率</param>
/// <param name="y">Yの拡大率</param>
/// <param name="z">Zの拡大率</param>
void AABB::Scaling(float x, float y, float z)
{
    mMax.x *= x;
    mMax.y *= y;
    mMax.z *= z;

    mMin.x *= x;
    mMin.y *= y;
    mMin.z *= z;
}

/// <summary>
/// スケーリング
/// </summary>
/// <param name="scale">拡大率</param>
void AABB::Scaling(float scale)
{
    Scaling(scale, scale, scale);
}

//------------------------------------------------//
//                    壁面                        //
//------------------------------------------------//

/// <summary>
/// コンストラクタ
/// </summary>
Wall::Wall()
    :mNormal(0,0,0)
    ,mStart(0,0,0)
    ,mEnd(0,0,0)
    ,mPlaneD(0.0f)
    ,mScale(0,0,0)
    ,mZRotate(0.0f)
    ,mSlopeAngle(0.0f)
    ,mIsSlope(false)
{
    for (int i = 0; i < 4; i++)
    {
        mWallVertex[i] = Vector3(0,0,0);
    }
}

/// <summary>
/// 点が壁に含まれているか？
/// </summary>
/// <param name="point">点の座標</param>
/// <returns>
/// true  : 含まれている
/// false : 含まれている
/// </returns>
bool Wall::Contains(const Vector3& point)
{
    //AABBにも含まれない場合はリターン
    if (!mWallAABB.Contains(point))
    {
        return false;
    }

    //平面の方程式　P・N + D = 0より
    if (Vector3::Dot(mNormal, point) + mPlaneD > 0.0f)
    {
        return false;
    }

    return true;
}

/// <summary>
/// z軸に水平壁を作成な壁を作成
/// </summary>
/// <param name="start">始点座標</param>
/// <param name="end">終点座標</param>
/// <param name="wallHeight">壁の高さ</param>
/// <returns>Wallのインスタンス</returns>
Wall Wall::CreateWall(const Vector3& start, const Vector3 end, float wallHeight)
{
    Wall temp;

    temp.mStart = start;
    temp.mEnd = end;
    temp.mIsSlope = false;

    //壁面の表面ベクトルを求める
    Vector3 wallLine = end - start;
    Vector3 nomalizedWallLine = wallLine;
    nomalizedWallLine.Normalize();

    //壁面の平面方程式
    temp.mNormal = Vector3::Cross(Vector3::UnitZ, nomalizedWallLine);
    temp.mPlaneD = -1.0f * Vector3::Dot(temp.mNormal, start);

    //壁面の4点を求める(start点の下→上→end点の上→下の順)
    Vector3 zMax(0.0f, 0.0f, start.z + wallHeight);

    temp.mWallVertex[0] = start;
    temp.mWallVertex[1] = start + zMax;
    temp.mWallVertex[2] = end   + zMax;
    temp.mWallVertex[3] = end;

    temp.mScale.x = 100.0f;
    temp.mScale.y = (end - start).Length();
    temp.mScale.z = wallHeight;

    //表示回転角と回転角
    Vector3 sgnVec = Vector3::Cross(Vector3::UnitX, nomalizedWallLine);
    float zAngle = acosf(Vector3::Dot(Vector3::UnitX, nomalizedWallLine)) - Math::PiOver2;

    temp.mZRotate = sgnVec.z > 0 ? zAngle : -zAngle + Math::Pi;
    temp.CalcAABB();

    return temp;
}

/// <summary>
/// 坂道の作成
/// </summary>
/// <param name="pos">座標</param>
/// <param name="zRotateDegree">斜面の角度</param>
/// <param name="horizonLen">長さ</param>
/// <param name="height">高さ</param>
/// <param name="slopeWidth">幅</param>
/// <returns>Wallのインスタンス</returns>
Wall Wall::CreateSlope(const Vector3& pos, Wall::AngleEnum zRotateDegree, float horizonLen, float height, float slopeWidth)
{
    Wall temp;
    Vector3 slopeLine, normalizedSlopeLine;


    // スロープ面を上から見下げた時、右側になるベクトル
    Vector3 slopeRightVec(0.0f, 1.0f, 0.0f);

    temp.mStart = Vector3(0, 0, height);
    temp.mEnd = Vector3(horizonLen, 0, 0);
    temp.mIsSlope = true;

    // スロープの表面ベクトルを求める
    slopeLine = temp.mEnd - temp.mStart;
    normalizedSlopeLine = slopeLine;
    normalizedSlopeLine.Normalize();

    // 壁面法線ベクトル求める
    temp.mNormal = Vector3::Cross(normalizedSlopeLine, slopeRightVec);
    temp.mNormal.Normalize();

    //傾斜角を求める
    temp.mSlopeAngle = acosf(Vector3::Dot(Vector3::UnitX, normalizedSlopeLine)) + Math::PiOver2;

    // スロープ面の4頂点を求める
    float halfWidth = slopeWidth * 0.5f;
    temp.mWallVertex[0] = temp.mStart - slopeRightVec * halfWidth;
    temp.mWallVertex[1] = temp.mStart + slopeRightVec * halfWidth;
    temp.mWallVertex[2] = temp.mEnd + slopeRightVec * halfWidth;
    temp.mWallVertex[3] = temp.mEnd - slopeRightVec * halfWidth;

    temp.mScale.x = 100.0f;
    temp.mScale.y = slopeWidth;
    temp.mScale.z = (temp.mEnd - temp.mStart).Length();

    temp.mZRotate = Math::ToRadians(static_cast<float>(zRotateDegree));

    // スロープ面の4頂点のローカルZ回転 + 平行移動
    Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(static_cast<float>(zRotateDegree)));
    Matrix4 trans = Matrix4::CreateTranslation(pos);
    Matrix4 mat = rot * trans;
    for (int i = 0; i < 4; i++)
    {
        temp.mWallVertex[i] = Vector3::Transform(temp.mWallVertex[i], mat);
    }
    // 法線を回転
    temp.mNormal = Vector3::Transform(temp.mNormal, rot);
    temp.mPlaneD = -1.0f * Vector3::Dot(temp.mNormal, temp.mWallVertex[0]);

    temp.CalcAABB();

    return temp;
}

/// <summary>
/// AABBの計算
/// </summary>
void Wall::CalcAABB()
{
    // x,y,zの最大値　最小値を調べる
    mWallAABB.mMin = mWallAABB.mMax = mWallVertex[0];
    for (int i = 1; i < 4; i++)
    {
        mWallAABB.mMin.x = Math::Min(mWallAABB.mMin.x, mWallVertex[i].x);
        mWallAABB.mMin.y = Math::Min(mWallAABB.mMin.y, mWallVertex[i].y);
        mWallAABB.mMin.z = Math::Min(mWallAABB.mMin.z, mWallVertex[i].z);

        mWallAABB.mMax.x = Math::Max(mWallAABB.mMax.x, mWallVertex[i].x);
        mWallAABB.mMax.y = Math::Max(mWallAABB.mMax.y, mWallVertex[i].y);
        mWallAABB.mMax.z = Math::Max(mWallAABB.mMax.z, mWallVertex[i].z);
    }
}

/// <summary>
/// AABB 同士の衝突
/// </summary>
/// <param name="a">自分のAABB</param>
/// <param name="b">相手のAABB</param>
/// <returns>
/// true  : 衝突している
/// false : 衝突していない
/// </returns>
bool Intersect(const AABB& a, const AABB& b)
{
    bool no = a.mMax.x < b.mMin.x ||
              a.mMax.y < b.mMin.y ||
              a.mMax.z < b.mMin.z ||
              b.mMax.x < a.mMin.x ||
              b.mMax.y < a.mMin.y ||
              b.mMax.z < a.mMin.z;

    // 一つもtrueがなければ、二つは衝突している
    return !no;
}

/// <summary>
/// 球体とAABBとの衝突
/// </summary>
/// <param name="s">球体のインスタンス</param>
/// <param name="box">AABBのインスタンス</param>
/// <returns>点との最小距離</returns>
bool Intersect(const Sphere& s, const AABB& box)
{
    float distSq = box.MinDistSq(s.mCenter);

    return distSq <= (s.mRadius * s.mRadius);
}

bool Intersect(const AABB& a, const Wall& wall, CollisionInfo& info)
{
    // めり込み戻りベクトル初期化
    info.mFixVec = Vector3(0, 0, 0);

    // AABB同士でぶつからないのであれば早期リターン
    if (!Intersect(a, wall.mWallAABB))
    {
        return false;
    }

    // 壁面のエッジと衝突したとき
    Vector3 fix(0, 0, 0);										// スタート点下　　　　スタート点上
    if (IntersectWallEdgeFix(a, wall.mWallVertex[0], wall.mWallVertex[1], fix))
    {
        info.mFixVec = fix;
        info.mCollisionPoint = wall.mWallVertex[0] + fix;
        return true;
    }
    // 終了点下　　　　　　　終了点上
    if (IntersectWallEdgeFix(a, wall.mWallVertex[3], wall.mWallVertex[2], fix))
    {
        info.mFixVec = fix;
        info.mCollisionPoint = wall.mWallVertex[3] + fix;
        return true;
    }

    // AABBの中心点は壁の外側にいるか？
    Vector3 boxCenter;
    boxCenter = (a.mMin + a.mMax) * 0.5f;

    float planeLength;
    planeLength = Vector3::Dot(wall.mNormal, boxCenter) + wall.mPlaneD;

    // 壁の外側にいるとき
    if (planeLength > 0.0f)
    {
        ///////////////////////////////////////////////////////////
        // 壁面に対して最も近い点を求める
        ///////////////////////////////////////////////////////////
        Vector3 nearestPoint;

        // 平面からの符号付距離を8点すべて求める
        std::vector<float> lengths;
        for (int i = 0; i < 8; i++)
        {
            lengths.push_back(Vector3::Dot(wall.mNormal, a.mVertex[i]));
        }
        // 壁面に対してAABBの最近傍点を見つける
        auto minitr = std::min_element(lengths.begin(), lengths.end());

        // ボックス中心点を通る、壁面と同じ法線の平面の方程式を立てる
        size_t nearestindex = std::distance(lengths.begin(), minitr);
        nearestPoint = a.mVertex[nearestindex];

        //// 最近傍点はWallのAABB内に含まれるか？
        //if (!Intersect(wall.mWallAABB, nearestPoint, info))
        //{
        //	return true;
        //}

        // 最近傍点と実際の壁の平面方程式で衝突判定を行う
        planeLength = Vector3::Dot(wall.mNormal, nearestPoint) + wall.mPlaneD;

        if (planeLength < 0.0f)
        {
            //ぶつかったので衝突点・押し戻し量を返す
            info.mFixVec = -1.0f * planeLength * wall.mNormal;
            info.mCollisionPoint = nearestPoint + info.mFixVec;

            return true;
        }
    }
    return false;
}

/// <summary>
/// AABBと点の衝突
/// </summary>
/// <param name="a">AABBのインスタンス</param>
/// <param name="movablepoint">点の座標</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool Intersect(const AABB& a, const Vector3& movablepoint, CollisionInfo& info)
{
    info.mCollisionPoint = Vector3(0, 0, 0);
    info.mFixVec = Vector3(0, 0, 0);

    if (!a.Contains(movablepoint))
    {
        return false;
    }
    float dx1 = movablepoint.x - a.mMin.z;
    float dx2 = movablepoint.x - a.mMin.x;
    float dy1 = movablepoint.y - a.mMax.x;
    float dy2 = movablepoint.y - a.mMin.y;
    float dz1 = movablepoint.z - a.mMax.y;
    float dz2 = movablepoint.z - a.mMax.z;

    // dx, dy, dzにそれぞれ絶対値が小さいほうをセットする
    float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
    float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
    float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

    // x,y,zのうち最も差が小さい軸で位置を調整
    if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
    {
        info.mFixVec.x = dx;
    }
    else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
    {
        info.mFixVec.y = dy;
    }
    else
    {
        info.mFixVec.z = dz;
    }
    info.mCollisionPoint = movablepoint + info.mFixVec;

    return true;
}

/// <summary>
/// 線分とAABBの衝突
/// </summary>
/// <param name="line">Lineのインスタンス</param>
/// <param name="box">AABBのインスタンス</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool Intersect(const Line& line, const AABB& box, CollisionInfo& outInfo)
{
    // 線分とAABBのベクトルを配列化 x,y,z -> [0],[1],[2]
    float p[3], d[3], min[3], max[3];
    p[0] = line.mLineStart.x;
    p[1] = line.mLineStart.y;
    p[2] = line.mLineStart.z;

    d[0] = line.mNormalizeDirection.x;
    d[1] = line.mNormalizeDirection.y;
    d[2] = line.mNormalizeDirection.z;

    min[0] = box.mMin.x;
    min[1] = box.mMin.y;
    min[2] = box.mMin.z;

    max[0] = box.mMax.x;
    max[1] = box.mMax.y;
    max[2] = box.mMax.z;

    // 線分の方程式パラメータtをfloat最小値、最大値で初期化
    float t = -FLT_MAX;
    float t_max = FLT_MAX;

    for (int i = 0; i < 3; ++i)
    {
        if (fabs(d[i]) < FLT_EPSILON)
        {
            if (p[i] < min[i] || p[i] > max[i])
            {
                return false;
            }
        }
        else
        {
            // スラブとの距離を算出
            // t1が近いスラブ、t2が遠いスラブとの距離
            float odd = 1.0f / d[i];
            float t1 = (min[i] - p[i]) * odd;
            float t2 = (max[i] - p[i]) * odd;
            if (t1 > t2)
            {
                float tmp = t1;
                t1 = t2;
                t2 = tmp;
            }

            if (t1 > t)
            {
                t = t1;
            }
            if (t2 < t_max)
            {
                t_max = t2;
            }
            // スラブ交差チェック
            if (t >= t_max)
            {
                return false;
            }
        }
    }
    Vector3 lineLength = line.mLineEnd - line.mLineStart;
    if (t < 0.0f || t > lineLength.Length())
    {
        return false;
    }

    //衝突情報
    outInfo.mCollisionPoint = line.mLineStart + t * (line.mNormalizeDirection);

    // 衝突法線は中心点から衝突点の一番大きい成分から判断する
    Vector3 centerPos, centerToColPos;
    centerPos = 0.5f * (box.mMin + box.mMax);
    centerToColPos = outInfo.mCollisionPoint - centerPos;

    float elem[3], normal[3];
    elem[0] = fabs(centerToColPos.x);
    elem[1] = fabs(centerToColPos.y);
    elem[2] = fabs(centerToColPos.z);

    //x,y,z最大のインデックス番号を返す
    int index = (elem[0] > elem[1]) ? (elem[0] > elem[2] ? 0 : 2) : (elem[1] > elem[2] ? 1 : 2);

    // 最大要素のみの成分残し後は0にし、その後正規化することで、x,y,zのうち最大要素のみ+1/-1のAABBの法線となる
    for (int i = 0; i < 3; i++)
    {
        normal[i] = (index == i ? elem[i] : 0);
    }
    outInfo.mHitNormal.x = normal[0];
    outInfo.mHitNormal.y = normal[1];
    outInfo.mHitNormal.z = normal[2];

    outInfo.mHitNormal.Normalize();

    return true;
}

/// <summary>
/// 壁と線分の衝突情報
/// </summary>
/// <param name="wall">壁のインスタンス</param>
/// <param name="line">線分のインスタンス</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool Intersect(const Wall& wall, const Line& line, CollisionInfo& info)
{
    // 線分とWallが作る平面が交差するか？
    Vector3 N, v1, v2;
    N = wall.mNormal;
    v1 = line.mLineStart - wall.mWallVertex[0];
    v2 = line.mLineEnd - wall.mWallVertex[0];

    float dotA = Vector3::Dot(v1, N);
    float dotB = Vector3::Dot(v2, N);

    const float epsiron = 0.000001f;
    dotA = fabs(dotA) > epsiron ? dotA : 0.0f;
    dotB = fabs(dotB) > epsiron ? dotB : 0.0f;


    // 平面と線分は衝突しない
    if (dotA * dotB > 0.0f)
    {
        return false;
    }

    // 線分と平面は交点を持つ
    info.mHitNormal = wall.mNormal;

    if (dotA == 0.0f && dotB == 0.0f)
    {
        //線分が平面に完全に含まれる場合
        info.mCollisionPoint = line.mLineStart;
        info.mFixVec = line.mLineStart - line.mLineEnd;

        return true;
    }

    // 平面との交点を求める
    Vector3 vec, planeHitPos;
    vec = line.mLineEnd - line.mLineStart;

    //衝突する内分の割合を求め、そこから衝突点を求める
    float ratio = fabs(dotA) / (fabs(dotA) + fabs(dotB));
    planeHitPos = vec * ratio + line.mLineStart;

    //衝突点がAABB内に含まれていたら衝突している 
    if (wall.mWallAABB.Contains(planeHitPos))
    {
        info.mFixVec = info.mCollisionPoint - line.mLineEnd;
        info.mCollisionPoint = planeHitPos;
        return true;
    }

    return false;
}

bool IntersectWallEdgeFix(const AABB& a, const Vector3& edgeLow, const Vector3& edgeHi, Vector3& fixVec)
{
    fixVec = Vector3(0, 0, 0);

    bool no = a.mMax.x < edgeLow.x ||
        a.mMax.y < edgeLow.y ||
        a.mMax.z < edgeLow.z ||
        edgeLow.x < a.mMin.x ||
        edgeLow.y < a.mMin.y ||
        edgeHi.z < a.mMin.z;
    if (no)
    {
        return false;
    }

    float dx1 = edgeLow.x - a.mMin.x;
    float dx2 = edgeLow.x - a.mMax.x;
    float dy1 = edgeLow.y - a.mMax.y;
    float dy2 = edgeLow.y - a.mMin.y;

    // dx, dy, dzにそれぞれ絶対値が小さいほうをセットする
    float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
    float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

    // x,y,zのうち最も差が小さい軸で位置を調整
    if (Math::Abs(dx) <= Math::Abs(dy))
    {
        if (Math::Abs(dx) > 0.0001f)
        {
            fixVec.x = dx;
        }
    }
    else
    {
        if (Math::Abs(dy) > 0.0001f)
        {
            fixVec.y = dy;
        }
    }
    // 修正があまりに小さいときはヒットしていないとみなす
    if (fixVec.LengthSq() < 0.0001f)
    {
        return false;
    }

    return true;
}

void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec)
{
    calcFixVec = Vector3(0, 0, 0);
    float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
    float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
    float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
    float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
    float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
    float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

    // dx, dy, dz には それぞれ1,2のうち絶対値が小さい方をセットする
    float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
    float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
    float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

    // x, y, zのうち最も差が小さい軸で位置を調整
    if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
    {
        calcFixVec.x = dx;
    }
    else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
    {
        calcFixVec.y = dy;
    }
    else
    {
        calcFixVec.z = dz;
    }
}
