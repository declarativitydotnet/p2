// -*- c-basic-offset: 2; related-file-name: "string.C" -*-
#ifndef __CLICK_STRING_H__
#define __CLICK_STRING_H__
#include <assert.h>

class String {
 public:
  
  // Call static_initialize() before any function which might deal with
  // Strings, and declare a String::Initializer in any file in which you
  // declare static global Strings.
  struct Initializer { Initializer(); };
  friend class String::Initializer;
  static void static_initialize();
  static void static_cleanup();

  inline String();
  inline String(const String &);
  String(const char *cc)		{ assign(cc, -1); }
  String(const char *cc, int l)		{ assign(cc, l); }
  String(const char *s1, const char *s2) { assign(s1, (s2>s1 ? s2-s1 : 0)); }
  explicit inline String(bool);
  explicit inline String(char);
  explicit inline String(unsigned char);
  explicit String(int);
  explicit String(unsigned);
  explicit String(long);
  explicit String(unsigned long);
  explicit String(double);
  inline ~String();
  
  static const String &null_string()	{ return *null_string_p; }
  static const String &out_of_memory_string() { return *oom_string_p; }
  static String garbage_string(int n);	// n garbage characters
  static String stable_string(const char *, int = -1); // stable read-only mem.
  static inline String stable_string(const char *, const char *);
  
  bool out_of_memory() const		{ return _memo == oom_memo; }
  
  int length() const			{ return _length; }
  const char *data() const		{ return _data; }
  
  typedef const char *const_iterator;
  typedef const_iterator iterator;
  const_iterator begin() const		{ return _data; }
  const_iterator end() const		{ return _data + _length; }

  inline bool data_shared() const;
  char *mutable_data();
  char *mutable_c_str();
  
  operator bool()			{ return _length != 0; }
  operator bool() const			{ return _length != 0; }
  
  const char *c_str() const;		// pointer returned is semi-transient
  const char *cc() const		{ return c_str(); }
  
  char operator[](int i) const		{ return _data[i]; }
  char at(int i) const			{ assert(i>=0&&i<_length); return _data[i]; }
  char front() const			{ return _data[0]; }
  char back() const			{ return _data[_length-1]; }
  int find_left(int c, int start = 0) const;
  int find_left(const String &s, int start = 0) const;
  int find_right(int c, int start = 0x7FFFFFFF) const;
  
  bool equals(const char *, int) const;
  // bool operator==(const String &, const String &);
  // bool operator==(const String &, const char *);
  // bool operator==(const char *, const String &);
  // bool operator!=(const String &, const String &);
  // bool operator!=(const String &, const char *);
  // bool operator!=(const char *, const String &);

  int compare(const char *, int) const;
  int compare(const String &x) const	{ return compare(x._data, x._length); }
  static inline int compare(const String &a, const String &b);
  // bool operator<(const String &, const String &);
  // bool operator<=(const String &, const String &);
  // bool operator>(const String &, const String &);
  // bool operator>=(const String &, const String &);

  String substring(const char *begin, const char *end) const;
  String substring(int pos, int len) const;
  String substring(int pos) const	{ return substring(pos, _length); }
  
  String lower() const;			// lowercase
  String upper() const;			// uppercase
  String printable() const;		// quote non-ASCII characters
  String trim_space() const;		// trim space from right
  
  inline String &operator=(const String &);
  inline String &operator=(const char *);

  void append(const char *, int len);
  inline void append(const char *, const char *);
  void append_fill(int c, int len);
  char *append_garbage(int len);
  inline String &operator+=(const String &);
  inline String &operator+=(const char *);
  inline String &operator+=(char);

  // String operator+(String, const String &);
  // String operator+(String, const char *);
  // String operator+(const char *, const String &);
  // String operator+(String, PermString);
  // String operator+(PermString, const String &);
  // String operator+(PermString, const char *);
  // String operator+(const char *, PermString);
  // String operator+(PermString, PermString);
  // String operator+(String, char);
  
 private:
   
  struct Memo {
    int _refcount;
    int _capacity;
    int _dirty;
    char *_real_data;
    
    Memo();
    Memo(char *, int, int);
    Memo(int, int);
    ~Memo();
  };
  
  mutable const char *_data;	// mutable for c_str()
  mutable int _length;
  mutable Memo *_memo;
  
  inline String(const char *, int, Memo *);
  
  inline void assign(const String &) const;
  void assign(const char *, int);
  inline void deref() const;
  void make_out_of_memory();
  
  static Memo *null_memo;
  static Memo *permanent_memo;
  static Memo *oom_memo;
  static String *null_string_p;
  static String *oom_string_p;
  
  static String claim_string(char *, int, int); // claim memory

  friend class StringAccum;
  
};


inline
String::String(const char *data, int length, Memo *memo)
  : _data(data), _length(length), _memo(memo)
{
  _memo->_refcount++;
}

inline void
String::assign(const String &s) const
{
  _data = s._data;
  _length = s._length;
  _memo = s._memo;
  _memo->_refcount++;
}

inline void
String::deref() const
{
  if (--_memo->_refcount == 0)
    delete _memo;
}

inline
String::String()
  : _data(null_memo->_real_data), _length(0), _memo(null_memo)
{
  _memo->_refcount++;
}

inline
String::String(bool b)
  : _data(b ? "true" : "false"), _length(b ? 4 : 5), _memo(permanent_memo)
{
  _memo->_refcount++;
}

inline
String::String(char c)
{
  assign(&c, 1);
}

inline
String::String(unsigned char c)
{
  assign(reinterpret_cast<char *>(&c), 1);
}

inline
String::String(const String &s)
{
  assign(s);
}

inline
String::~String()
{
  deref();
}

inline bool
String::data_shared() const
{
  return !_memo->_capacity || _memo->_refcount != 1;
}

inline String
String::stable_string(const char *s1, const char *s2)
{
  if (s1 < s2)
    return String::stable_string(s1, s2 - s1);
  else
    return String();
}

inline String
String::substring(const char *s1, const char *s2) const
{
  if (s1 < s2 && s1 >= _data && s2 <= _data + _length)
    return String(s1, s2 - s1, _memo);
  else
    return String();
}

inline int
String::compare(const String &a, const String &b)
{
  return a.compare(b);
}

inline bool
operator==(const String &s1, const String &s2)
{
  return s1.equals(s2.data(), s2.length());
}

inline bool
operator==(const char *cc1, const String &s2)
{
  return s2.equals(cc1, -1);
}

inline bool
operator==(const String &s1, const char *cc2)
{
  return s1.equals(cc2, -1);
}

inline bool
operator!=(const String &s1, const String &s2)
{
  return !s1.equals(s2.data(), s2.length());
}

inline bool
operator!=(const char *cc1, const String &s2)
{
  return !s2.equals(cc1, -1);
}

inline bool
operator!=(const String &s1, const char *cc2)
{
  return !s1.equals(cc2, -1);
}

inline bool
operator<(const String &s1, const String &s2)
{
  return s1.compare(s2.data(), s2.length()) < 0;
}

inline bool
operator<=(const String &s1, const String &s2)
{
  return s1.compare(s2.data(), s2.length()) <= 0;
}

inline bool
operator>(const String &s1, const String &s2)
{
  return s1.compare(s2.data(), s2.length()) > 0;
}

inline bool
operator>=(const String &s1, const String &s2)
{
  return s1.compare(s2.data(), s2.length()) >= 0;
}


inline String &
String::operator=(const String &s)
{
  if (&s != this) {
    deref();
    assign(s);
  }
  return *this;
}

inline String &
String::operator=(const char *cc)
{
  deref();
  assign(cc, -1);
  return *this;
}

inline void
String::append(const char *s1, const char *s2)
{
  if (s1 < s2)
    append(s1, s2 - s1);
}

inline String &
String::operator+=(const String &s)
{
  append(s._data, s._length);
  return *this;
}

inline String &
String::operator+=(const char *cc)
{
  append(cc, -1);
  return *this;
}

inline String &
String::operator+=(char c)
{
  append(&c, 1);
  return *this;
}

inline String
operator+(String s1, const String &s2)
{
  s1 += s2;
  return s1;
}

inline String
operator+(String s1, const char *cc2)
{
  s1.append(cc2, -1);
  return s1;
}

inline String
operator+(const char *cc1, const String &s2)
{
  String s1(cc1);
  s1 += s2;
  return s1;
}

inline String
operator+(String s1, char c2)
{
  s1.append(&c2, 1);
  return s1;
}

int hashcode(const String &);

// find methods

inline const char *find(const char *a, const char *b, char c)
{
  while (a < b && *a != c)
    a++;
  return a;
}

inline const char *find(const String &s, char c)
{
  return find(s.begin(), s.end(), c);
}

#endif
