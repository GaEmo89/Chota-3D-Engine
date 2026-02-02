#pragma once
#include <memory>
#include <string>

class ModelRenderer;

// Minimal renderer component: references an already-loaded model resource
struct MeshRendererComponent
{
    std::shared_ptr<ModelRenderer> Model; // shared resource
};
