#include "server_internal_data.h"

namespace modbus4qt {

ServerInternalData::ServerInternalData(QObject *parent)
    : QObject{parent}
{

}

//-----------------------------------------------------------------------------

void
ServerInternalData::addCoil(quint16 index)
{
    if (coils_.contains(index))
    {
        return;
    }

    emit beforeCoilAdded(index);

    coils_[index] = false;

    emit coilAdded(index);
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addCoils(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addCoil(i);
    }
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addDescreteInput(quint16 index)
{
    if (descreteInputs_.contains(index))
    {
        return;
    }

    descreteInputs_[index] = false;
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addDescreteInputs(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addDescreteInput(i);
    }
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addHoldingRegister(quint16 index)
{
    if (holdingRegisters_.contains(index))
    {
        return;
    }

    holdingRegisters_[index] = 0;
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addHoldingRegisters(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addHoldingRegister(i);
    }
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addInputRegister(quint16 index)
{
    if (inputRegisters_.contains(index))
    {
        return;
    }

    inputRegisters_[index] = 0;
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addInputRegisters(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addInputRegister(i);
    }

}

//-----------------------------------------------------------------------------

Coils*
modbus4qt::ServerInternalData::coils()
{
    return &coils_;
}

//-----------------------------------------------------------------------------

const Coils& ServerInternalData::constCoils() const
{
    return coils_;
}

//-----------------------------------------------------------------------------

const Coils&
ServerInternalData::descreteInputs() const
{
    return descreteInputs_;
}

//-----------------------------------------------------------------------------

const QMap<quint16, quint16>& ServerInternalData::holdingRegisters() const
{
    return holdingRegisters_;
}

//-----------------------------------------------------------------------------

const QMap<quint16, quint16>& ServerInternalData::inputRegisters() const
{
    return inputRegisters_;
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF

