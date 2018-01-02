#include <stdexcept>

namespace Magma
{
	class InvalidStringError : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	/// <summary>
	///     Class that wraps a C string. This class uses the UTF-8 encoding.
	/// </summary>
	class String
	{
	public:
		/// <summary>
		///		Constructs an empty string
		/// </summary>
		String();
		~String() noexcept;

		/// <summary>
		///		Constructs a string from a UTF-8 C string
		/// </summary>
		/// <param name="cstring">UTF-8 C string</param>
		/// <exception cref="InvalidStringError">Thrown if C string contains invalid UTF-8</exception>
		String(const char* utf8CString);

		/// <summary>
		///		Constructs a string from another string (copy constructor)
		/// </summary>
		/// <param name="string">String to copy</param>
		String(const String& string);

		/// <summary>
		///		Constructs a string from another string, inutilizing the other string (move constructor)
		/// </summary>
		/// <param name="string">String to move</param>
		String(String&& string) noexcept;

		/// <summary>
		///		Copies a UTF-8 C string into this string
		/// </summary>
		/// <param name="utf8CString">UTF-8 C string</param>
		/// <exception cref="InvalidStringError">Thrown if C string contains invalid UTF-8</exception>
		/// <returns>This</returns>
		String& operator=(const char* utf8CString);

		/// <summary>
		///		Copies a string into this string (copy assignment operator)
		/// </summary>
		/// <param name="string">String to copy</param>
		/// <returns>This</returns>
		String& operator=(const String& string);

		/// <summary>
		///		Moves a string into this string (move assignment operator)
		/// </summary>
		/// <param name="string">String to move</param>
		/// <returns>This</returns>
		String& operator=(String&& string) noexcept;

		/// <summary>
		///		Gets the underlying C string (char array) from this string
		/// </summary>
		/// <returns>Char array used in this string</returns>
		inline const char* CString() const { return m_data; }

		/// <summary>
		///		Appends a unicode character to this string
		/// </summary>
		/// <param name="character">Character unicode point</param>
		/// <exception cref="InvalidCharacterError">Thrown if unicode character is invalid</exception>
		void AppendChar(char32_t character);

		/// <summary>
		///		Appends a UTF-8 character to this string and returns the character size in bytes
		/// </summary>
		/// <param name="character">UTF-8 character address</param>
		/// <exception cref="InvalidCharacterError">Thrown if UTF-8 character is invalid</exception>
		/// <returns>UTF-8 character size</returns>
		size_t AppendChar(const char* character);

		/// <summary>
		///		Appends a UTF-8 C string to this string
		/// </summary>
		/// <param name="utf8CString">UTF-8 C string to append</param>
		/// <exception cref="InvalidStringError">Thrown if C string contains invalid UTF-8</exception>
		void AppendString(const char* utf8CString);

		/// <summary>
		///		Appends a string to this string
		/// </summary>
		/// <param name="string">String to append</param>
		void AppendString(const String& string);

		/// <summary>
		///		Removes the last character in the string
		/// </summary>
		/// <exception cref="std::out_of_range">Thrown when the string is already empty</exception>
		void Pop();

		/// <summary>
		///		Clears the string
		/// </summary>
		void Clear();

		/// <summary>
		///		Appends this string and another, and returns the result
		/// </summary>
		/// <param name="rhs">String to append</param>
		/// <returns>Appended strings</returns>
		String operator+(const String& rhs);

		/// <summary>
		///		Appends a string to this string
		/// </summary>
		/// <param name="rhs">String to append</param>
		/// <returns>This</returns>
		String& operator+=(const String& rhs);

		/// <summary>
		///		Appends this string and a UTF-8 C string, and returns the result
		/// </summary>
		/// <param name="rhs">UTF-8 C string to append</param>
		/// <exception cref="InvalidStringError">Thrown if C string contains invalid UTF-8</exception>
		/// <returns>Appended strings</returns>
		String operator+(const char* rhs);

		/// <summary>
		///		Appends a UTF-8 C string to this string
		/// </summary>
		/// <param name="rhs">UTF-8 C string to append</param>
		/// <exception cref="InvalidStringError">Thrown if C string contains invalid UTF-8</exception>
		/// <returns>This</returns>
		String& operator+=(const char* rhs);

	private:
		size_t m_length;
		size_t m_size;
		char* m_data;
	};

	namespace UTF8
	{
		class InvalidCharacterError : public std::runtime_error
		{
		public:
			using std::runtime_error::runtime_error;
		};

		/// <summary>
		///		Checks if a UTF-8 C string is valid
		/// </summary>
		/// <param name="utf8String">UTF-8 C string to check</param>
		/// <returns>True if valid, otherwise false</returns>
		bool IsValid(const char* utf8String);

		/// <summary>
		///		Gets the size a unicode character would have if encoded in UTF-8
		/// </summary>
		/// <param name="character">Character to check</param>
		/// <exception cref="InvalidCharacterError">Thrown if unicode character is invalid</exception>
		/// <returns>Character UTF-8 size</returns>
		size_t GetCharSize(char32_t character);

		/// <summary>
		///		Gets the size of an UTF-8 character
		/// </summary>
		/// <param name="utf8Char">Address of UTF-8 character</param>
		/// <exception cref="InvalidCharacterError">Thrown if UTF-8 character has invalid prefix</exception>
		/// <returns>Character size</returns>
		size_t GetCharSize(const char* utf8Char);

		/// <summary>
		///		Gets the length of an UTF-8 C string (in characters, not bytes)
		/// </summary>
		/// <param name="utf8String">C String address</param>
		/// <exception cref="InvalidCharacterError">Thrown if a UTF-8 character has invalid prefix</exception>
		/// <returns>C String length</returns>
		size_t GetStringLength(const char* utf8String);

		/// <summary>
		///		Converts a UTF-8 character to a unicode character
		/// </summary>
		/// <param name="utf8Char">Address of UTF-8 character</param>
		/// <exception cref="InvalidCharacterError">Thrown if UTF-8 character has invalid prefix</exception>
		/// <seealso cref="FromUnicode"/>
		/// <returns>Unicode character</returns>
		char32_t ToUnicode(const char* utf8Char);

		/// <summary>
		///		Converts a unicode character to a UTF-8 character
		/// </summary>
		/// <param name="character">Character to be converted</param>
		/// <param name="utf8Char">UTF-8 character output address (maximum character size is 4 bytes)</param>
		/// <exception cref="InvalidCharacterError">Thrown if unicode character is invalid</exception>
		/// <seealso cref="ToUnicode"/>
		/// <returns>UTF-8 character size</returns>
		size_t FromUnicode(char32_t character, char* utf8Char);
	}
}
