#pragma once
#include "storage/istorage.h"

namespace hb
{
class DocEngine;

DocEngine& InitDocEngine(const IStoragePtr& storage);
DocEngine& GetDocEngine();
}
