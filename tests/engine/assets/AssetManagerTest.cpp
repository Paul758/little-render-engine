#include <catch2/catch_test_macros.hpp>

#include "engine/assets/AssetManager.h"
#include "engine/assets/TextureLoader.h"

#include "tests/helpers/OpenGLTestContext.h"

TEST_CASE("TextureLoader loads image data")
{
    const auto path = std::filesystem::path(TEST_ASSET_DIR) / "textures" / "brick.png";

    ImageData image = TextureLoader::load(path);

    REQUIRE(image.width > 0);
    REQUIRE(image.height > 0);
    REQUIRE(image.channels == 4);
    REQUIRE_FALSE(image.pixels.empty());
    REQUIRE(image.pixels.size() == static_cast<std::size_t>(image.width * image.height * image.channels));
}

TEST_CASE("Asset Manager successfully loads a texture")
{
    OpenGLTestContext context;
    AssetManager assetManager(TEST_ASSET_DIR);

    Texture2D& brickTextureFirst = assetManager.loadTexture("textures/brick.png");
    Texture2D& brickTextureSecond = assetManager.loadTexture("textures/brick.png");


    REQUIRE(&brickTextureFirst != nullptr);
    REQUIRE(&brickTextureSecond != nullptr);
    REQUIRE(&brickTextureFirst == &brickTextureSecond);

}

TEST_CASE("TextureLoader throws when image does not exist")
{
    const auto path = std::filesystem::path(TEST_ASSET_DIR) / "textures" / "does_not_exist.png";

    REQUIRE_THROWS_AS(TextureLoader::load(path), std::runtime_error);

}