#include <catch2/catch_test_macros.hpp>

#include "engine/scene/SceneFile.h"

struct TemporarySceneFile
{
    std::filesystem::path path;

    ~TemporarySceneFile()
    {
        std::error_code error;
        std::filesystem::remove(path, error);
    }
};

TEST_CASE("SceneFile correctly writes and reads files")
{
    TemporarySceneFile sceneFile{std::filesystem::temp_directory_path() / "little-renderer-scene-file-test.json"};

    const std::string jsonString = R"(
    {
        "entities": [
            {
                "id": 42,
                "components": {
                    "Name": {
                        "name" : "Player"
                    },
                    "Health": {
                        "Current Health": 42.0,
                        "Alive": true
                    },
                    "Transform": {
                        "Position": {
                            "x": 1.0,
                            "y": 2.0,
                            "z": 3.0
                        },
                        "Scale": {
                            "x": 4.0,
                            "y": 5.0,
                            "z": 6.0
                        },
                        "Rotation": {
                            "w": 7.0,
                            "x": 8.0,
                            "y": 9.0,
                            "z": 10.0
                        }
                    },
                    "Test": [
                        "Test",
                        1.0,
                        2.0,
                        true
                    ]
                }
            }
        ]
    }
    )";

    INFO("Path: " << sceneFile.path.string());
    INFO("Exists: " << std::filesystem::exists(sceneFile.path));
    INFO("Parent exists: " << std::filesystem::exists(sceneFile.path.parent_path()));

    SceneFile::write(sceneFile.path, jsonString);
    REQUIRE(SceneFile::read(sceneFile.path) == jsonString);
}