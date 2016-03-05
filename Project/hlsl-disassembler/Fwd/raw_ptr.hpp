#pragma once

// Foward Declaration
namespace stx
{
namespace filesystem
{
typedef std::basic_string<uint8_t, std::char_traits<uint8_t>, std::allocator<uint8_t> > raw;
typedef std::basic_ifstream<uint8_t, std::char_traits<uint8_t> > rfstream;

typedef std::shared_ptr<raw> raw_ptr;
};
};