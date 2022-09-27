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

    // �q�b�g�|�C���g�o�[�p�̒��_�z����쐬
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

    // �X�P�[���s��̌v�Z
    Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth())  * mScale.x,
                                            static_cast<float>(mTexture->GetHeight()) * mScale.y,
                                            1.0f);

    // �X�N���[���ʒu�̍s��v�Z
    Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffsetPos.x - (RENDERER->GetScreenWidth() * 0.5f),
                                                         (RENDERER->GetScreenHeight() * 0.5f) - mOffsetPos.y,
                                                          0.0f));

    // ���[���h�ϊ��s��̌v�Z
    Matrix4 world = scaleMat * transMat;

    // ���_�z����A�N�e�B�u�ɃZ�b�g
    mHitPointBarVerts->SetActive();

    Shader* useShader = nullptr;
    useShader = GraphicResourceManager::FindUseShader(ShaderTag::TILEMAP);

    // �V�F�[�_�[�ɕK�v�ȏ����Z�b�g
    useShader->SetActive();
    useShader->SetMatrixUniform("uWorldTransform", world);
    useShader->SetIntUniform("uTileIndex", 0);
    useShader->SetVector2Uniform("uTileSetSplitNum", Vector2(1, 1));
    useShader->SetFloatUniform("uAlpha", mAlpha);

    // �e�N�X�`�����A�N�e�B�u�ɃZ�b�g
    mTexture->SetActive();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
