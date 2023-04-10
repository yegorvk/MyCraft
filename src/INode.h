//
// Created by egorv on 4/9/2023.
//

#ifndef SHITCRAFT_INODE_H
#define SHITCRAFT_INODE_H

#include "Drawable.h"
#include "UpdateDelegate.h"
#include "EventConsumer.h"

class INode : public Drawable, public UpdateDelegate, public EventConsumer {};

#endif //SHITCRAFT_INODE_H
