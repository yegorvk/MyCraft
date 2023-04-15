//
// Created by egorv on 4/9/2023.
//

#ifndef SHITCRAFT_INODE_H
#define SHITCRAFT_INODE_H

#include "core/Drawable.h"
#include "UpdateDelegate.h"
#include "core/EventConsumer.h"

class INode : public Drawable, public UpdateDelegate, public EventConsumer {};

#endif //SHITCRAFT_INODE_H
