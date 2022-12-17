#pragma once

#include "Constants/ItemId.h"
#include "ItemSchema.h"
#include "PODs/EconItem.h"
#include "PODs/SharedObjectTypeCache.h"
#include "VirtualMethod.h"

namespace csgo
{

struct SOID {
    std::uint64_t id;
    std::uint32_t type;
    std::uint32_t padding;
};

class EconItemView;
enum class Team;

namespace pod { struct SharedObject; }
namespace pod { struct ClientSharedObjectCache; }
namespace pod { struct CSPlayerInventory; }

class CSPlayerInventory : public VirtualCallableFromPOD<CSPlayerInventory, pod::CSPlayerInventory> {
public:
    VIRTUAL_METHOD(void, soCreated, 0, (SOID owner, pod::SharedObject* object, int event), (owner, object, event))
    VIRTUAL_METHOD(void, soUpdated, 1, (SOID owner, pod::SharedObject* object, int event), (owner, object, event))
    VIRTUAL_METHOD(void, soDestroyed, 2, (SOID owner, pod::SharedObject* object, int event), (owner, object, event))
    VIRTUAL_METHOD_V(void, removeItem, 15, (ItemId itemID), (itemID))

    auto getSOC() const noexcept
    {
        return *reinterpret_cast<pod::ClientSharedObjectCache**>(getThis() + WIN32_LINUX(0xB4, 0xF8));
    }

    auto getAccountID() const noexcept
    {
        return *reinterpret_cast<std::uint32_t*>(getThis() + WIN32_LINUX(0x8, 0x10));
    }

    auto getSOID() const noexcept
    {
        return *reinterpret_cast<SOID*>(getThis() + WIN32_LINUX(0x8, 0x10));
    }
};

}

inline csgo::pod::SharedObjectTypeCache* getItemBaseTypeCache(const csgo::CSPlayerInventory& inventory, std::uintptr_t createBaseTypeCacheFn) noexcept
{
    if (const auto soc = csgo::ClientSharedObjectCache{ VirtualCallable{ inventory.getInvoker(), std::uintptr_t(inventory.getSOC()) }, createBaseTypeCacheFn }; soc.getThis() != 0)
        return soc.findBaseTypeCache(1);
    return nullptr;
}
