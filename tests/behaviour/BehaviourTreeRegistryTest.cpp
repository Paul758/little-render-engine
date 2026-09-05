#include <catch2/catch_test_macros.hpp>

#include "behaviour/BehaviourTreeRegistry.h"
#include "behaviour/player/PlayerMovementBTreeBuilder.h"
#include "behaviour/composites/Sequence.h"

TEST_CASE("BehaviourTreeRegistry registers a new tree")
{
    BehaviourTreeRegistry treeRegistry;

    auto firstTree = std::make_unique<BehaviourTree>(std::make_unique<Sequence>());
    auto secondTree = std::make_unique<BehaviourTree>(std::make_unique<Sequence>());

    BehaviourTreeId firstId = treeRegistry.add(std::move(firstTree));
    BehaviourTreeId secondId = treeRegistry.add(std::move(secondTree));

    REQUIRE(firstId != secondId);
}

TEST_CASE("BehaviourTreeRegistry returns the registered tree")
{
    BehaviourTreeRegistry registry;

    auto tree = std::make_unique<BehaviourTree>(std::make_unique<Sequence>());

    BehaviourTree* originalTree = tree.get();

    BehaviourTreeId id = registry.add(std::move(tree));

    REQUIRE(registry.get(id) == originalTree);
}

TEST_CASE("BehaviourTreeRegistry returns the correct tree for each ID")
{
    BehaviourTreeRegistry registry;

    auto firstTree = std::make_unique<BehaviourTree>(std::make_unique<Sequence>());
    auto secondTree = std::make_unique<BehaviourTree>(std::make_unique<Sequence>());

    BehaviourTree* firstPtr = firstTree.get();
    BehaviourTree* secondPtr = secondTree.get();

    BehaviourTreeId firstId = registry.add(std::move(firstTree));
    BehaviourTreeId secondId = registry.add(std::move(secondTree));

    REQUIRE(registry.get(firstId) == firstPtr);
    REQUIRE(registry.get(secondId) == secondPtr);
}

TEST_CASE("BehaviourTreeRegistry returns nullptr for an invalid ID")
{
    BehaviourTreeRegistry registry;

    BehaviourTree* tree = registry.get(1234);

    REQUIRE(tree == nullptr);
}