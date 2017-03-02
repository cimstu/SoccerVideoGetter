#ifndef TINYHTM_H
#define TINYHTM_H

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

class TiHtmDocument;
class TiHtmElement;
class TiHtmComment;
class TiHtmUnknown;
class TiHtmText;
class TiHtmDeclaration;

class TiHtmParsingData;

/// record the location
struct TiHtmCursor
{
	TiHtmCursor() { clear(); }
	void clear() { row = column = -1; }

	int row;
	int column;
};


/// the base class of whole tinyhtml
class TiHtmBase
{
	friend class TiHtmNode;
	friend class TiHtmElement;
	//friend class TiHtmDocumnet;
public:
	TiHtmBase() {}
	virtual ~TiHtmBase() {}

	virtual void print(FILE *cfile, int depth) const = 0;
	virtual void printValue(FILE *cfile, int depth) const = 0;
	virtual const char *parse(const char *p, TiHtmParsingData *data) = 0;

	int row() const { return location.row; }
	int column() const { return location.row; }

	/// Expands entities in a string. Note this should not contian the tag '<', '>', etc.
	static void encodeString(const std::string& str, std::string* out); // not write code

protected:
	static bool isWhiteSpace(char c);
	static bool isWhiteSpace(int c) 
	{ 
		if (c < 256)
			return isWhiteSpace((char)c);
		return false;
	}
	static const char *skipWhiteSpace(const char *p);

	/** Read a html name into the string provided. Return a pointer
	    pointer just past the last character fo the name.
	    return NULL if has error.
	*/
	static const char *readName(const char *p, std::string *name);
	/// 读取element中的text，直到遇到endTag为止
	static const char *readText(const char *p, std::string *text, bool trimWhiteSpace, const char *endTag, bool ignoreCase);
	/// 把转义字符转化为原来的字符，比如"&lt;"转化为 '<'
	static const char *getEntity(const char *p, char *value);
	/// 从输入流中获取一个字符，有可能转换转义字符
	static const char *getChar(const char *p, char *value);
	
	/// if tge is p's prefix, return true, else return false
	static bool stringEqual(const char *p, const char *tag, bool ignoreCase);
	
	// record the location message
	TiHtmCursor location;
	
	static int isAlpha(unsigned char c);
	static int isAlnum(unsigned char c);
	static int toLower(int c) { return tolower(c); }

private:
	TiHtmBase(const TiHtmBase &);			// not allowd
	void operator=(const TiHtmBase &base); // not allowd

	struct Entity
	{
		const char *str;
		unsigned int strLength;
		const char chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6
	};

	static Entity entity[NUM_ENTITY]; // defined in file tinyhtmparser.cpp
};

class TiHtmNode : public TiHtmBase
{
	friend class TiHtmElement;
	//friend class TiHtmDocument;
public:

	/// the type of html nodes
	enum NodeType
	{
		TINYHTM_DOCUMENT,
		TINYHTM_ELEMENT,
		TINYHTM_COMMENT,
		TINYHTM_UNKNOWN,
		TINYHTM_TEXT,
		TINYHTM_DECLARATION,
		TINYHTM_TYPTCOUNT
	};
	
	virtual ~TiHtmNode();
	
	/**
		the different meaning of value
		
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
	*/
	const std::string getValue() const { return value; }
	const char  *getValueStr() const { return value.c_str(); }
	void setValue(const char *_value) { value = _value; }
	void setValue(const std::string &_value) { value = _value; }
	
	/// delete alll the child nodes
	void clear();
	
	TiHtmNode *getParent() 					{ return parent; }
	const TiHtmNode *getParent() const 	{ return parent; }
	TiHtmNode *getFirstChild()				{ return firstChild; }
	const TiHtmNode *getFirstChild() const { return firstChild; }
	TiHtmNode *getLastChild() 				{ return lastChild; }
	const TiHtmNode *getLastChild() const  { return lastChild; }
	
	const TiHtmNode *getFirstChild(const char *_value) const;
	TiHtmNode *getFirstChild(const char *_value)
	{
		return const_cast<TiHtmNode *>((const_cast<const TiHtmNode*>(this))->getFirstChild(_value));
	}
	const TiHtmNode *getFirstChild(const std::string &_value) const { return getFirstChild(_value.c_str()); }
	TiHtmNode *getFirstChild(const std::string &_value) { return getFirstChild(_value.c_str()); }
	
	const TiHtmNode *getLastChild(const char *_value) const;
	TiHtmNode *getLastChild(const char *_value)
	{
		return const_cast<TiHtmNode *>((const_cast<const TiHtmNode*>(this))->getLastChild(_value));
	}
	const TiHtmNode *getLastChild(const std::string &_value) const { return getLastChild(_value.c_str()); }
	TiHtmNode *geLastChild(const std::string &_value) { return getLastChild(_value.c_str()); }
	
	TiHtmNode *insertEndChild(const TiHtmNode &addNode);
	TiHtmNode *linkEndChild(TiHtmNode *pnode);

	TiHtmNode *insertBeforeChild(TiHtmNode *beforeNode, const TiHtmNode &addNode);
	TiHtmNode *insertAfterChild(TiHtmNode *afterNode, const TiHtmNode &addNode);
	TiHtmNode *replaceChild(TiHtmNode *replaceNode, const TiHtmNode &withNode);
	
	/// Delete a child of this node
	bool removeChild(TiHtmNode *removeNode);
	
	const TiHtmNode *previousSibling() const { return prev; }
	TiHtmNode *previousSibling()				  { return prev; }
	const TiHtmNode *previousSibling(const char *_value) const;
	TiHtmNode *previousSibling(const char *_value)
	{
		return const_cast<TiHtmNode *>((const_cast<const TiHtmNode *>(this))->previousSibling(_value));
	}
	
	/// Using std::string
	const TiHtmNode *previousSibling(const std::string &_value) const { return previousSibling(_value.c_str()); }
	TiHtmNode *previousSibling(const std::string &_value) 			  { return previousSibling(_value.c_str()); }
	const TiHtmNode *nextSibling(const std::string &_value) const	  { return nextSibling(_value.c_str()); }
	TiHtmNode *nextSibling(const std::string &_value)					  { return nextSibling(_value.c_str()); }
	
	const TiHtmNode *nextSibling() const { return next; }
	TiHtmNode *nextSibling()				  { return next; }
	const TiHtmNode *nextSibling(const char *_value) const;
	TiHtmNode *nextSibling(const char *_value)
	{
		return const_cast<TiHtmNode *>((const_cast<const TiHtmNode *>(this))->nextSibling(_value));
	}
	
	/// Some convenience function
	const TiHtmElement *nextSiblingElement() const;
	TiHtmElement *nextSiblingElement()
	{
		return const_cast<TiHtmElement *>((const_cast<const TiHtmNode *>(this))->nextSiblingElement());
	}
	
	const TiHtmElement *nextSiblingElement(const char *_value) const;
	TiHtmElement *nextSiblingElement(const char *_value)
	{
		return const_cast<TiHtmElement *>((const_cast<const TiHtmNode *>(this))->nextSiblingElement(_value));
	}
	
	/// Some convenience function using std::string
	const TiHtmElement *nextSiblingElement(const std::string &_value) const { return nextSiblingElement(_value.c_str()); }
	TiHtmElement *nextSiblingElement(const std::string &_value) 				 { return nextSiblingElement(_value.c_str()); }
	
	/// Convenience function to get through elements.
	const TiHtmElement *firstChildElement() const;
	TiHtmElement *firstChildElement()
	{
		return const_cast<TiHtmElement *>((const_cast<const TiHtmNode *>(this))->firstChildElement());
	}
	
	const TiHtmElement *firstChildElement(const char *_value) const;
	TiHtmElement *firstChildElement(const char *_value)
	{
		return const_cast<TiHtmElement *>((const_cast<const TiHtmNode *>(this))->firstChildElement(_value));
	}
	
	/// Some convenience function using std::string
	const TiHtmElement *firstChildElement(const std::string &_value) const { return firstChildElement(_value.c_str()); }
	TiHtmElement *firstChildElement(const std::string &_value)			   { return firstChildElement(_value.c_str()); }
	
	/** the type of this node
	    they are TINYHTM_DOCUMENT,TINYHTM_ELEMENT,TINYHTM_COMMENT,
		TINYHTM_UNKNOWN,TINYHTM_TEXT,TINYHTM_DECLARATION
	*/
	int getType() const { return type; }
	
	/// return true if this node has no children
	bool noChildren() const { return !firstChild; }
	
	virtual const TiHtmDocument 		*toDocument() const { return NULL; }
	virtual const TiHtmElement  		*toElement()  const { return NULL; }
	virtual const TiHtmComment  		*toComment()  const { return NULL; }
	virtual const TiHtmUnknown  		*toUnknown()  const { return NULL; }
	virtual const TiHtmText	   		*toText()	   const { return NULL; }
	virtual const TiHtmDeclaration  *toDeclaration() const { return NULL; }
	
	virtual TiHtmDocument 		*toDocument() { return NULL; }
	virtual TiHtmElement  		*toElement()  { return NULL; }
	virtual TiHtmComment  		*toComment()  { return NULL; }
	virtual TiHtmUnknown  		*toUnknown()  { return NULL; }
	virtual TiHtmText	   			*toText()	   { return NULL; }
	virtual TiHtmDeclaration    *toDeclaration() { return NULL; }
	
	/// Create an exact duplicate of this node and return it. The memory must be deleted by the caller
	virtual TiHtmNode *clone() const = 0;
	
	/// Copy to allocated object
	void copyTo(TiHtmNode *target) const;
	
	/// Figure out what is at *p, and parse it. Returns null if it is not an html node.
	/// 解析输入流中字符是什么，然后new相应的类型并返回
	TiHtmNode *identify(const char *p);
	
protected:
	TiHtmNode(NodeType _type);
	
	TiHtmNode *parent;
	NodeType type;
	TiHtmNode *firstChild;
	TiHtmNode *lastChild;
	
	std::string value;
	
	TiHtmNode *prev;
	TiHtmNode *next;

private:
	TiHtmNode(const TiHtmNode &);			// not implemented
	void operator=(const TiHtmNode &base); // not implemented
};

// Only used by Attribute::Query functions
enum
{
	TIHTM_SUCCESS,
	TIHTM_NO_ATTRIBUTE,
	TIHTM_WRONG_TYPE
};

// 属性节点
class TiHtmAttribute : public TiHtmBase
{
	friend class TiHtmAttributeSet;

public:
	TiHtmAttribute() : TiHtmBase() { prev = next = NULL; }
	TiHtmAttribute(const std::string& _name, const std::string& _value)
	{
		name = _name;
		value = _value;
		prev = next = NULL;
	}
	TiHtmAttribute(const char* _name, const char* _value) : name(_name), value(_value)
	{
		prev = next = NULL;
	}
	
	const char* getNameStr() const { return name.c_str(); }
	const char* getValueStr() const { return value.c_str(); }
	const std::string& getName() const { return name; }
	const std::string& getValue() const { return value; }
	
	/// Returns the value of this attribute, converted to an interger.
	int intValue() const;
	/// Returns the value of this attribute, converted to an double.
	double doubleValue() const;
	
	/// examing the value string. It is an alternative to the IntValue() method with error checking.
	/// 将value值转换为int/doulle存放到_value中
	int queryIntValue(int* _value) const;
	int queryDoubleValue(double* _value) const;
	
	void setName(const char* _name) { name = _name; }
	void setName(const std::string& _name) { name = _name; }
	void setValue(const char* _value) { value = _value; }
	void setValue(const std::string& _value) { value = _value; }
	
	/// Set the value from an integer.
	void setIntValue(int _value);
	/// Set the value from a double.
	void setDoubleValue(double _value);
	
	/// Get the next sibling attribute in the Attribute. Returns NULL at end.
	const TiHtmAttribute* getNext() const;
	TiHtmAttribute* getNext()
	{
		return const_cast<TiHtmAttribute*>((const_cast<const TiHtmAttribute *>(this))->getNext());
	}
	
	/// Get the previous sibling attribute in the Attribute. Returns NULL at beginning.
	const TiHtmAttribute* getPrevious() const;
	TiHtmAttribute* getPrevious()
	{
		return const_cast<TiHtmAttribute*>((const_cast<const TiHtmAttribute*>(this))->getPrevious());
	}
	
	bool operator==(const TiHtmAttribute& rhs) const { return rhs.name == name; }
	bool operator< (const TiHtmAttribute& rhs) const { return name < rhs.name; }
	bool operator> (const TiHtmAttribute& rhs) const { return name > rhs.name; }
	
	/// Attribute parseing starts: first letter of the name
	///						returns: the next char after the value end quote.
	/// 读取name和value值，比如version="1.0"，name="version"，value="1.0"，一次只能读取一个name和value的值
	/// value的值不包括"1.0"的引号部分，当然，加入""中在含有""时，value就包含""了
	virtual const char* parse(const char* p, TiHtmParsingData* data);
	
	/// Print this Attribute to a FILE stream.
	virtual void print(FILE *cfile, int depth) const
	{
		print(cfile, depth, 0);
	}
	void print(FILE* cfile, int depth, std::string* str) const;
	
	virtual void printValue(FILE *cfile, int depth) const;
	
private:
	/// Not allow copy assign
	TiHtmAttribute(const TiHtmAttribute&);
	TiHtmAttribute& operator=(const TiHtmAttribute& base);
	
	std::string name;
	std::string value;
	TiHtmAttribute* prev;
	TiHtmAttribute* next;
};

// 属性集合
class TiHtmAttributeSet
{
public:
	TiHtmAttributeSet();
	~TiHtmAttributeSet();
	
	/// Add an arrtibute to this set
	void add(TiHtmAttribute* arrtibute);
	/// Remove an arrtibute from this set
	void remove(TiHtmAttribute* arrtibute);
	
	const TiHtmAttribute* first() const { return (sentinel.next == &sentinel) ? NULL : sentinel.next; }
	TiHtmAttribute* first() 		 { return (sentinel.next == &sentinel) ? NULL : sentinel.next; }
	const TiHtmAttribute* last() const  { return (sentinel.prev == &sentinel) ? NULL : sentinel.prev; }
	TiHtmAttribute* last() 			 { return (sentinel.prev == &sentinel) ? NULL : sentinel.prev; }
	
	TiHtmAttribute* find(const char* _name) const;
	TiHtmAttribute* findOrCreate(const char* _name);
	
	TiHtmAttribute* find(const std::string& _name) const;
	TiHtmAttribute* findOrCreate(const std::string& _name);

private:
	/// Not allow copy assign
	TiHtmAttributeSet(const TiHtmAttributeSet&);
	TiHtmAttributeSet& operator=(const TiHtmAttributeSet& base);
	
	TiHtmAttribute sentinel;
};

class TiHtmElement : public TiHtmNode
{
public:
	TiHtmElement(const char *_value);
	
	// Using std::string
	TiHtmElement(const std::string &_value);
	
	TiHtmElement(const TiHtmElement &);
	//TiHtmElement &operator=(const TiHtmElement &base);

	virtual ~TiHtmElement();
	
	const char* attribute(const char* name) const;
	/// 如果*i不为NULL时，会将value转换为int型并存到i指向的内存中
	const char* attribute(const char* name, int* i) const;
	const char* attribute(const char* name, double* d) const;
	
	/// QueryIntAttribute examines the attribute, store in _value
	int QueryIntAttribute(const char* name, int* _value) const;
	/// QueryUnsignedAttribute examines the attribute, store in _value
	int queryUnsignedAttribute(const char* name, unsigned* _value) const;
	/// QueryBoolAttribute examines the attribute, store in _value
	int queryBoolAttribute(const char* name, bool* _value) const;
	/// QueryDoubleAttribute examines the attribute, store in _value
	int queryDoubleAttribute(const char* name, double* _value) const;
	/// QueryFloatAttribute examines the attribute, store in _value
	int queryFloatAttribute(const char* name, float* _value) const
	{
		double d;
		int result = queryDoubleAttribute(name, &d);
		if (result == TIHTM_SUCCESS)
		{
			*_value = (float)d;
		}
		return result;
	}
	
	/// std::string 相关操作
	const std::string* attribute(const std::string& name) const;
	const std::string* attribute(const std::string& name, int* i) const;
	const std::string* attribute(const std::string& name, double* d) const;
	int queryIntAttribute(const std::string& name, int* value) const { return queryIntAttribute(name.c_str(), value); }
	int queryDoubleAttribute(const std::string& name, double* value) const { queryDoubleAttribute(name.c_str(), value); }
	/// std::string 相关操作
	void setAttribute(const std::string& name, const std::string& value) { setAttribute(name.c_str(), value.c_str()); }
	void setAttribute(const std::string& name, int value) { setAttribute(name.c_str(), value); }
	void setDoubleAttribute(const std::string& name, double value) { setDoubleAttribute(name.c_str(), value); }
	
	/// Sets an attribute of name to a string value. This attribute will created if it does not exist.
	void setAttribute(const char* name, const char* value);
	/// Sets an attribute of name to a int value. This attribute will created if it does not exist.
	void setAttribute(const char* name, int value);
	/// Sets an attribute of name to a double value. This attribute will created if it does not exist.
	void setDoubleAttribute(const char* name, double value);
	
	/// Deletes an attribute with the given name.
	void removeAttribute(const char *name);
	void removeAttribute(const std::string& name) { removeAttribute(name.c_str()); }
	
	/// Access the first attribute in this element.
	const TiHtmAttribute* firstAttribute() const { return attributeSet.first(); }
	TiHtmAttribute* firstAttribute() 			{ return attributeSet.first(); }
	/// Access the last attribute in this element.
	const TiHtmAttribute* lastAttribute() const { return attributeSet.last(); }
	TiHtmAttribute* lastAttribute()			 { return attributeSet.last(); }
	
	/// 如果element的第一个child不是text，则返回null，否则返回text的字符串
	const char *getText() const;
	
	/// Creates a new Element and returns it - the returned element is a copy.
	virtual TiHtmNode *clone() const;
	
	/// Print the Element to a FILE stream
	virtual void print(FILE *cfile, int depth) const;
	/// 打印Value
	virtual void printValue(FILE *cfile, int depth) const;
	
	/// Parse() declared in class TiHTmBase
	/// Attribute parsing starts: next char past '<'
	///					  returns: next char past '>'
	virtual const char *parse(const char *p, TiHtmParsingData *data);
	
	virtual const TiHtmElement *toElement() const { return this; }
	virtual TiHtmElement *toElement()				{ return this; }

protected:
	void copyTo(TiHtmElement *target) const;
	void clearThis(); // like clear, bur initializes 'this' object as well
	
	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char *readValue(const char *in, TiHtmParsingData *prevData);

private:
	TiHtmAttributeSet attributeSet;
};

// An HTML comment
class TiHtmComment : public TiHtmNode 
{
public:
	/// Construct an empty comment.
	TiHtmComment() : TiHtmNode(TiHtmNode::TINYHTM_COMMENT) {}
	/// Construct an empty comment for test.
	TiHtmComment(const char* _value) : TiHtmNode(TiHtmNode::TINYHTM_COMMENT) 
	{
		setValue(_value);
	}
	TiHtmComment(const TiHtmComment&);
	TiHtmComment& operator=(const TiHtmComment& base);
	
	virtual ~TiHtmComment() {}
	
	/// Return a copy of this comment
	virtual TiHtmNode* clone() const;
	
	/// Write this Comment to a FILE stream
	virtual void print(FILE *cfile, int depth) const;
	/// 打印Value
	virtual void printValue(FILE *cfile, int depth) const;
	
	/// Pasesing starts: at the ! of the !--
	///			 returns: next char past '>'
	virtual const char* parse(const char* p, TiHtmParsingData* data);
	
	virtual const TiHtmComment* toComment() const { return this; }
	virtual 	TiHtmComment* toComment()	{ return this; }

protected:
	void copyTo(TiHtmComment* target) const;
};

class TiHtmText : public TiHtmNode
{
	friend class TiHtmElement;
public:
	TiHtmText(const char *_value) : TiHtmNode(TiHtmNode::TINYHTM_TEXT)
	{
		setValue(_value);
	}
	TiHtmText(const std::string &_value) : TiHtmNode(TiHtmNode::TINYHTM_TEXT)
	{
		setValue(_value);
	}
	virtual ~TiHtmText() {}
	
	TiHtmText(const TiHtmText &copy) : TiHtmNode(TiHtmNode::TINYHTM_TEXT) { copy.copyTo(this); }
	TiHtmText &operator=(const TiHtmText &base) { base.copyTo(this); return *this; }
	
	/// Write this text to a FILE stream
	virtual void print(FILE *cfile, int depth) const;
	/// 打印value
	virtual void printValue(FILE *cfile, int depth) const;
	
	/// Parse the text
	virtual const char *parse(const char *p, TiHtmParsingData *data);
	
	virtual const TiHtmText *toText() const { return this; }
	virtual TiHtmText *toText()		  		 { return this; }

protected:
	// internal use, create a new elemnet node and returns it
	virtual TiHtmText *clone() const;
	
	void copyTo(TiHtmText *target) const { TiHtmNode::copyTo(target); }
	
	bool blank() const;
};

// An Unknown node of html file
class TiHtmUnknown : public TiHtmNode
{
public:
	TiHtmUnknown() : TiHtmNode(TiHtmNode::TINYHTM_UNKNOWN), valueIsVisible(false) {}
	virtual ~TiHtmUnknown() {}
	
	TiHtmUnknown(const TiHtmUnknown& copy) : TiHtmNode(TiHtmNode::TINYHTM_UNKNOWN) { copy.copyTo(this); }
	TiHtmUnknown& operator=(const TiHtmUnknown& copy) 
	{ 
		copy.copyTo(this); 
		return *this; 
	}
	
	/// Creates a copy of this Unknown and returns it
	virtual TiHtmUnknown* clone() const;
	
	/// Set the valueIsVisible, printValue是否打印Unknown的text
	void setValueVisible(bool flag) { valueIsVisible = flag; }
	
	/// Print this Unknow to a FILE stream
	virtual void print(FILE* cfile, int depth) const;
	/// Onlyu print this Unknow text to a FILE stream
	virtual void printValue(FILE* cfile, int depth) const;
	
	virtual const char* parse(const char* p, TiHtmParsingData* data);
	
	virtual const TiHtmUnknown* toUnknown() const { return this; }
	virtual 	TiHtmUnknown* toUnknown()		{ return this; }

protected:
	void copyTo(TiHtmUnknown* target) const;
	
	bool valueIsVisible;
};

class TiHtmDocument : public TiHtmNode
{
public:

	TiHtmDocument();
	TiHtmDocument(const char *filename);
	TiHtmDocument(std::string filename);

	TiHtmDocument(const TiHtmDocument &copy);
	TiHtmDocument &operator=(const TiHtmDocument &copy);
	
	virtual ~TiHtmDocument() {}

	bool loadFile();
	bool loadFile(const char *filename);
	bool loadFile(const std::string &filename);
	bool loadFile(FILE *file);
	
	bool saveFile() const;
	bool saveFile(const char *filename) const;
	bool saveFile(const std::string &filename) const;
	bool saveFile(FILE *file) const;

	/// Start parse the document file
	virtual const char *parse(const char *p, TiHtmParsingData *data = NULL);
	
	/// Get the root element
	const TiHtmElement *rootElement() const { return firstChildElement(); }
	TiHtmElement *rootElement()				 { return firstChildElement(); }
	
	/// The tab size funciton
	void setTabSize(int _tabsize) { tabsize = _tabsize; }
	int tabSize() const 			 { return tabsize; }
	
	void print() const { print(stdout, 0); }
	virtual void print(FILE *cfile, int depth = 0) const;

	void printValue() const { printValue(stdout, 0); }
	virtual void printValue(FILE *cfile, int depth = 0) const;
	
private:
	/// Internal use
	virtual TiHtmNode *clone() const;
	void copyTo(TiHtmDocument *target) const;
	
	int tabsize;
};

#endif

