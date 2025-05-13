#ifndef HAFMAN_H
#define HAFMAN_H

typedef unsigned char byte;
#define interface struct

interface IInputStream {
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);

// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);

#endif HAFMAN_H
