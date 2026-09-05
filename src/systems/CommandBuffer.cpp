#include "systems/CommandBuffer.h"

void CommandBuffer::clear()
{
    moveCommands.clear();
    jumpCommands.clear();
}