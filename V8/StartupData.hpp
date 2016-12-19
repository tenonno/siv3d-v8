#pragma once

namespace JS
{

	class StartupDataBlob
	{
	public:


		s3d::FilePath path;

		s3d::ByteArray buffer;

		v8::StartupData data;


		StartupDataBlob(const s3d::FilePath &path)
			: path(path), buffer(path)
		{


			this->data.raw_size = static_cast<int>(this->buffer.size());
			this->data.data = reinterpret_cast<const char *>(this->buffer.data());


		}

		~StartupDataBlob()
		{
			this->buffer.release();
		}

		v8::StartupData *get()
		{
			return &this->data;
		}

	};




	struct StartupData
	{
		s3d::String natives_blob;
		s3d::String snapshot_blob;

		static StartupData Default;
	};


	StartupData StartupData::Default
	{
		Format(L"/", V8_NATIVES_BLOB),
		Format(L"/", V8_SNAPSHOT_BLOB)
	};


}