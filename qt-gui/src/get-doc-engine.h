#pragma once
#include "storage/storage.h"

namespace hb
{
class DocEngine;

DocEngine& InitDocEngine(const storage::StoragePtr& storage);
DocEngine& GetDocEngine();
}
