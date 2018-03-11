#include "cmediafile.h"

CMediaFile::CMediaFile(const int id, const QString file_path, const qint64 file_size, const QDateTime created, const QString file_genre)
    : CDatabaseMember(id), _file(QFileInfo(file_path)), _size(file_size), _created(created), _genre(file_genre)
{
    if(!_file.isFile()) throw NO_FILE;
    if(!_file.exists() || _file.size() != file_size || _file.created() != _created) throw INVALID;
    if(!_file.isReadable()) throw NO_READABLE;

}

CMediaFile::CMediaFile(const int id, const QFileInfo file_info)
    : CDatabaseMember(id), _file(file_info)
{
    /// do
}

QFileInfo CMediaFile::file() const
{
    return _file;
}

