#ifndef __QIODEV_H__
#define __QIODEV_H__

#ifdef __cplusplus
extern "C" {
#endif

// 基类
class QIODevice
{
	enum OpenMode
	{
		NotOpen		= 0x00,
		ReadOnly,
		WriteOnly,
		ReadWrite,
		Append,
		Truncate 	= 0x8,
		Text 		= 0x10,
		Unbuffered 	= 0x20,
		NeweOnly 	= 0x40
	};
public:
	QIODevice();
	~QIODevice();

	bool open(OpenMode mode);
	void close();

	bool getChar(char * c);
//	bool isOpen();
//	bool isReadable();

	bool putChar(char c);
//	bool isWritable();

//	int read(char * data, int maxSize);

	virtual bool writeData(const char * data, int maxSize) = 0;
	virtual int  readData(char * data, int maxSize) = 0;
//signal:
	void readyRead();



};

#ifdef __cplusplus
}
#endif

#endif