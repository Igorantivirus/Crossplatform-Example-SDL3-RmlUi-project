#ifndef SDL_WRAPPER_FILEWORKER_HPP
#define SDL_WRAPPER_FILEWORKER_HPP

#include <string>

#include <SDL3/SDL_iostream.h>

namespace sdlwrapper
{

    enum FileWorkerMode : unsigned char
    {
        read = 0b00000001,   // чтение
        write = 0b00000010,  // запись
        app = 0b00000100,    // добавление
        binary = 0b00001000, // бинарно
        dblmode = 0b00010000 // чтение + запись
    };

    class FileWorker
    {
    public:
        FileWorker() = default;
        FileWorker(const std::string &path, const FileWorkerMode mode)
        {
            open(path, mode);
        }
        FileWorker(const std::string &path, const int mode)
        {
            open(path, mode);
        }
        ~FileWorker()
        {
            close();
        }

        bool open(const std::string &path, const int mode)
        {
            return open(path, static_cast<FileWorkerMode>(mode));
        }
        bool open(const std::string &path, const FileWorkerMode mode)
        {
            char strMode[4] = {};
            fillMode(strMode, mode);
            io_ = SDL_IOFromFile(path.c_str(), strMode);
            return static_cast<bool>(io_);
        }

        bool close()
        {
            if (io_)
                return SDL_CloseIO(io_);
            return false;
        }

        bool isOpen() const
        {
            return static_cast<bool>(io_);
        }

        std::string readAll()
        {
            const auto sz = SDL_GetIOSize(io_);
            std::string buf(static_cast<size_t>(sz), '\0');
            SDL_ReadIO(io_, buf.data(), buf.size());
            return buf;
        }
        std::string readNext(const Sint64 size)
        {
            std::string buf(static_cast<size_t>(size), '\0');
            SDL_ReadIO(io_, buf.data(), buf.size());
            return buf;
        }

        bool write(const std::string &str)
        {
            size_t bytesWritten = SDL_WriteIO(io_, str.c_str(), str.size());
            return bytesWritten == str.size();
        }

        bool Seek(long offset, const SDL_IOWhence type)
        {
            Sint64 result = SDL_SeekIO(io_, offset, type);
            return result != -1;
        }
        size_t Tell()
        {
            Sint64 position = SDL_TellIO(io_);
            if (position == -1)
                return 0;
            return static_cast<size_t>(position);
        }

        template <typename Type>
        friend FileWorker &operator<<(FileWorker &out, const Type &value)
        {
            if constexpr (std::is_same_v<Type, std::string> || std::is_same_v<Type, const char *> || std::is_same_v<Type, char *>)
                out.write(value);
            else
                out.write(std::to_string(value));
            return out;
        }

    private:
        SDL_IOStream *io_ = nullptr;

    private:
        static void fillMode(char modeStr[4], const FileWorkerMode fmode)
        {
            std::memset(modeStr, 0, 4);
            unsigned char ind = 0;
            modeStr[ind++] = (fmode & FileWorkerMode::read) ? 'r' : (fmode & FileWorkerMode::write) ? 'w'
                                                                : (fmode & FileWorkerMode::app)     ? 'a'
                                                                                                    : '\0';
            if (fmode & FileWorkerMode::binary)
                modeStr[ind++] = 'b';
            if (fmode & FileWorkerMode::dblmode)
                modeStr[ind++] = '+';
        }
    };

} // sdlwrapper

#endif // SDL_WRAPPER_FILEWORKER_HPP
