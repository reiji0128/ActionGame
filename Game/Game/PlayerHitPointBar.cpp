#include "PlayerHitPointBar.h"
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

PlayerHitPointBar::PlayerHitPointBar(const Vector2& offset, const Vector2& scale, float alpha)
{
    mOffsetPos = offset;
    mScale = scale;
    mAlpha = alpha;

    // ヒットポイントバー用の頂点配列を作成
    mHitPointBarVerts = new VertexArray();
    mHitPointBarVerts->CreateHitPointBarVAO();

   /* mTexture = new Texture;
    mTexture->Load("");*/
}

PlayerHitPointBar::~PlayerHitPointBar()
{
}

void PlayerHitPointBar::Draw()
{

    // スケール行列の計算
    Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth())  * mScale.x,
                                            static_cast<float>(mTexture->GetHeight()) * mScale.y,
                                            1.0f);

    // スクリーン位置の行列計算
    Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffsetPos.x - (RENDERER->GetScreenWidth() * 0.5f),
                                                         (RENDERER->GetScreenHeight() * 0.5f) - mOffsetPos.y,
                                                          0.0f));

    // ワールド変換行列の計算
    Matrix4 world = scaleMat * transMat;

    // 頂点配列をアクティブにセット
    mHitPointBarVerts->SetActive();

    Shader* useShader = nullptr;
    useShader = GraphicResourceManager::FindUseShader(ShaderTag::TILEMAP);

    // シェーダーに必要な情報をセット
    useShader->SetActive();
    useShader->SetMatrixUniform("uWorldTransform", world);
    useShader->SetIntUniform("uTileIndex", 0);
    useShader->SetVector2Uniform("uTileSetSplitNum", Vector2(1, 1));
    useShader->SetFloatUniform("uAlpha", mAlpha);

    // テクスチャをアクティブにセット
    mTexture->SetActive();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
