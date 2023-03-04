/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#include "types.h"

#include <algorithm>

namespace modbus4qt
{

//-----------------------------------------------------------------------------

ProtocolDataUnit::ProtocolDataUnit()
    : functionCode(0)
{
    std::fill(data, data + PDUDataMaxSize, 0);
}

//-----------------------------------------------------------------------------

ProtocolDataUnit::ProtocolDataUnit(const ProtocolDataUnit& rhv)
    : functionCode(rhv.functionCode)
{
    std::copy(rhv.data, rhv.data + PDUDataMaxSize, data);
}

//-----------------------------------------------------------------------------

ProtocolDataUnit&
ProtocolDataUnit::operator=(const ProtocolDataUnit& rhv)
{
    functionCode = rhv.functionCode;
    std::copy(rhv.data, rhv.data + PDUDataMaxSize, data);

    return *this;
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
