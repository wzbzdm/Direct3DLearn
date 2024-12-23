#pragma once

#include "Drawable.h"
#include "Transformable.h"

typedef union {

} CreateType;

typedef struct {
	CreateType type;
} CreateConfig;

class Shape3DBase : public Drawable, public Transformable {
public:
	Shape3DBase() = default;
};
