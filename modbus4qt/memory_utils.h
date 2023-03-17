#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <cstdint>
#include <QVector>

namespace modbus4qt
{

//!
//! \brief The WordRec union represents word of two bytes
//!
union WordRec
{
    uint16_t word;
    uint8_t bytes[2];
};

//!
//! \brief Return hi byte of the word
//! \param word - word
//! \return hi byte of the word
//!
uint8_t hi(uint16_t word);

//!
//! \brief Return low byte of the word
//! \param word - word
//! \return lo byte of the word
//!
uint8_t low(uint16_t word);

//!
//! \brief Swap two bytes in word and return result
//! \param word - word
//! \return The ward with swaped bytes
//!
uint16_t swap(uint16_t word);

//!
//! \brief Convert 2 bytes word from local byte order to net byte order
//! \param word - word to be converted
//! \return Converted word
//!
uint16_t host2net(uint16_t word);

//!
//! \brief Convert 2 bytes word from net byte order to local byte order
//! \param word - word to be converted
//! \return Converted word
//!
uint16_t net2host(uint16_t word);

} // namespace modbus4qt

#endif // MEMORY_UTILS_H
