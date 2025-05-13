/** 5. Алгоритм сжатия данных Хаффмана
 * Напишите две функции для создания архива из одного файла и извлечения файла из архива.
 * Метод архивирует данные из потока original
 * void Encode(IInputStream& original, IOutputStream& compressed);
 * Метод восстанавливает оригинальные данные
 * void Decode(IInputStream& compressed, IOutputStream& original);
 * где:
 * typedef char byte;
 * interface IInputStream {
 * // Возвращает false, если поток закончился
 * virtual bool Read(byte& value) = 0;
 * };
 * interface IOutputStream {
 * virtual void Write(byte value) = 0;
 * };
 * В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных. Дерево
 * Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
 * В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также
 * включающий файл Huffman.h. Тестирующая программа выводит размер сжатого файла в
 * процентах от исходного.
 * Пример минимального решения:
 * #include "Huffman.h"
 * static void copyStream(IInputStream&input, IOutputStream& output) {
 * byte value;
 * while(input.Read(value)) { output.Write(value); }
 * }
 * void Encode(IInputStream& original, IOutputStream& compressed) {
 * copyStream(original, compressed);
 * }
 * void Decode(IInputStream& compressed, IOutputStream& original) {
 * copyStream(compressed, original);
 * }
**/

#include "Huffman.h"
#include <vector>
#include <array>
#include <queue>

class BitWriter {
public:
    void WriteBit(unsigned char bit) {
        if (bitCount % 8 == 0) {
            buffer.push_back(0);
        }
        if (bit) {
            buffer.back() |= 1 << (7 - bitCount % 8);
        }
        ++bitCount;
    }
    void WriteByte(unsigned char b) {
        if (bitCount % 8 == 0) {
            buffer.push_back(b);
        } else {
            int offset = bitCount % 8;
            buffer.back() |= b >> offset;
            buffer.push_back(static_cast<unsigned char>(b << (8 - offset)));
        }
        bitCount += 8;
    }
    void Flush(IOutputStream& out) {
        for (unsigned char b : buffer) {
            out.Write(static_cast<byte>(b));
        }
    }
private:
    std::vector<unsigned char> buffer;
    size_t bitCount = 0;
};

class BitReader {
public:
    explicit BitReader(IInputStream& in_) : in(in_) {}
    bool ReadBit(unsigned char& bit) {
        if (bitsLeft == 0) {
            if (!in.Read(current)) {
                return false;
            }
            bitsLeft = 8;
        }
        bit = (current >> (bitsLeft - 1)) & 1;
        bitsLeft--;
        return true;
    }
    bool ReadByte(unsigned char& byte) {
        byte = 0;
        for (int i = 0; i < 8; ++i) {
            unsigned char b;
            if (!ReadBit(b)) return false;
            byte = static_cast<unsigned char>((byte << 1) | b);
        }
        return true;
    }
private:
    IInputStream& in;
    unsigned char current = 0;
    int bitsLeft = 0;
};

struct Node {
    unsigned char ch{};
    size_t freq{};
    Node* left{};
    Node* right{};
    Node(unsigned char c, size_t f) : ch(c), freq(f) {}
    Node(Node* l, Node* r) : ch(0), freq(l->freq + r->freq), left(l), right(r) {}
    ~Node() { delete left; delete right; }
};

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->freq > b->freq;
    }
};

struct Code {
    size_t bits = 0;
    size_t len = 0;
};

static void BuildCodes(const Node* node, Code table[256], size_t bits, size_t len) {
    if (!node) {
        return;
    }

    if (!node->left && !node->right) {
        table[node->ch].bits = bits;
        table[node->ch].len = len ? len : 1;
        return;
    }
    BuildCodes(node->left, table, bits << 1, size_t(len + 1));
    BuildCodes(node->right, table, (bits << 1) | 1, size_t(len + 1));
}

static void SerializeTree(const Node* node, BitWriter& bw) {
    if (!node) {
        return;
    }
    if (!node->left && !node->right) {
        bw.WriteBit(1);
        bw.WriteByte(node->ch);
    } else {
        bw.WriteBit(0);
        SerializeTree(node->left, bw);
        SerializeTree(node->right, bw);
    }
}

static Node* DeserializeTree(BitReader& br) {
    unsigned char bit;
    if (!br.ReadBit(bit)) {
        return nullptr;
    }
    if (bit) {
        unsigned char c;
        br.ReadByte(c);
        return new Node(c, 0);
    } else {
        Node* left = DeserializeTree(br);
        Node* right = DeserializeTree(br);
        return new Node(left, right);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<unsigned char> data;
    std::array<size_t, 256> freq{};
    byte value;
    while (original.Read(value)) {
        unsigned char c = static_cast<unsigned char>(value);
        freq[c]++;
        data.push_back(c);
    }

    size_t originalSize = data.size();
    for (int i = 0; i < 8; ++i) {
        compressed.Write(static_cast<byte>((originalSize >> (i * 8)) & 255));
    }
    if (originalSize == 0) {
        return;
    }

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    for (int i = 0; i < 256; ++i) {
        if (freq[i]) pq.push(new Node(static_cast<unsigned char>(i), freq[i]));
    }
    if (pq.size() == 1) {
        Node* only = pq.top(); pq.pop();
        pq.push(new Node(only, new Node('\0', 0)));
    }
    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a, b));
    }
    Node* root = pq.top();
    Code table[256]{};
    BuildCodes(root, table, 0, 0);

    BitWriter bw;
    SerializeTree(root, bw);
    for (unsigned char c : data) {
        Code cd = table[c];
        for (int i = cd.len - 1; i >= 0; --i) {
            bw.WriteBit((cd.bits >> i) & 1);
        }
    }
    bw.Flush(compressed);
    delete root;
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    size_t originalSize = 0;
    for (int i = 0; i < 8; ++i) {
        byte b;
        if (!compressed.Read(b)) {
            return;
        }
        originalSize |= size_t(static_cast<unsigned char>(b)) << (i * 8);
    }
    if (originalSize == 0) {
        return;
    }

    BitReader br(compressed);
    Node* root = DeserializeTree(br);
    if (root == nullptr) {
        return;
    }
    if (root->left == nullptr && root->right == nullptr) {
        for (size_t i = 0; i < originalSize; ++i) {
            original.Write(static_cast<byte>(root->ch));
        }
        delete root;
        return;
    }
    Node* node = root;
    size_t produced = 0;
    unsigned char bit;
    while (produced < originalSize && br.ReadBit(bit)) {
        node = bit ? node->right : node->left;
        if (!node) {
            break;
        }
        if (node->left == nullptr && node->right == nullptr) {
            original.Write(static_cast<byte>(node->ch));
            ++produced;
            node = root;
        }
    }
    delete root;
}
