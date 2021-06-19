#include "MHXX/Enemy/Enemy.hpp"
#include "MHXX/Strings/Enemy.hpp"

namespace MHXX {

const char* getEnemyStr( EmID& _id )
{
    for ( auto& a : EnemyStr )
        if ( a.ID.ID == _id.ID && a.ID.IDSub == _id.IDSub )
            return a.Name;

    return "EmID not found!";
}

} // MHXX
