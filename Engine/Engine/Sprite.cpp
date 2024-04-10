#include "EngineCore.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "AssetManager.h"
#include "TextureAsset.h"
#include "Entity.h"

IMPLEMENT_DYNAMIC_CLASS(Sprite);

void Sprite::Destroy() 
{
    Component::Destroy();
    texture = nullptr;
}

void Sprite::Update() 
{
    Component::Update();

    const Transform& transform = owner->GetTransform();
    size = IVec2(transform.scale * IVec2(sourceRect.w, sourceRect.h)).Abs();
    const IVec2 pos = transform.position - size / 2;
    targetRect = 
    {
        pos.x,
        pos.y,
        size.x,
        size.y
    };

	SetFlipFromScale(transform.scale);
}

void Sprite::SerializeCreate(RakNet::BitStream& bitStream) const
{
    Component::SerializeCreate(bitStream);

    if (texture != nullptr)
    {
        STRCODE _id = texture->GetUid();
        bitStream.Write(_id);
    }
    else
    {
        bitStream.Write(0);
    }
}

void Sprite::DeserializeCreate(RakNet::BitStream& bitStream)
{
    Component::DeserializeCreate(bitStream);

    STRCODE texAsset = 0;
    bitStream.Read(texAsset);
    TextureAsset* asset = AssetManager::Instance().GetAsset<TextureAsset>(texAsset);
    SetTextureAsset(asset);
}

void Sprite::Load(json::JSON& node) 
{
    Component::Load(node);
    if (node.hasKey("Texture")) 
    {
        const std::string tex_asset_guid = node["Texture"].ToString();
        LOG("Trying to load Texture: " << tex_asset_guid);
        SetTextureAsset(AssetManager::Instance().GetAsset<TextureAsset>(tex_asset_guid));
    }
}

void Sprite::SetSourceRect(const SDL_Rect rect)
{
    sourceRect = rect;
}

void Sprite::SetTextureAsset(TextureAsset* texAsset) 
{
    texture = texAsset;

    if (texAsset != nullptr)
    {
        size = texAsset->GetDimensions();
        sourceRect = { 0, 0, size.x, size.y };
    }
}

void Sprite::Render()
{
#ifndef STRIP_NULL_CHECKS
    if (texture == nullptr)
    {
        LOG("No pretty picture :(");
        return;
    }
#endif

    const auto texture = this->texture->GetTexture();
    SDL_SetTextureColorMod(texture, filterColor.r, filterColor.g, filterColor.b);
    SDL_RenderCopyEx(
        &RenderSystem::Instance().GetRenderer(),
        texture,
        &sourceRect,
        &targetRect,
        (double)owner->GetTransform().rotation,
        nullptr,
        flip
    );
    SDL_SetTextureColorMod(texture, 255, 255, 255);
}

SDL_RendererFlip Sprite::SetFlipFromScale(const Vec2 scale)
{
    flip = static_cast<SDL_RendererFlip>((scale.x < 0) | ((scale.y < 0) << 1));

    return flip;
}

void Sprite::SetFilterColor(int r, int g, int b, int a)
{
	filterColor.r = static_cast<Uint8>(r);
	filterColor.g = static_cast<Uint8>(g);
	filterColor.b = static_cast<Uint8>(b);
	filterColor.a = static_cast<Uint8>(a);
}
