namespace fk
{
	template <class T>
	void Patch::write(const T& value)
	{
		memcpy_s(_address + position, sizeof(T), &value, sizeof(T));
		position += sizeof(T);
	}
}
