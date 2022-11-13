#pragma once

template<class T, class E>
class TextureContainer
{
public:
	virtual T* GetTexture(E TexType) const = 0;
};

