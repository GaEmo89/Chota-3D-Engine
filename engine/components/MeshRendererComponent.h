#pragma once
#include <memory>

class ModelRenderer;

struct MeshRendererComponent
{
    std::shared_ptr<ModelRenderer> Model;
};
