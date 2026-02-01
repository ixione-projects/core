#ifndef JSON_H
#define JSON_H

typedef enum {
	String,
	Number,
	Boolean,
	Null,
	Object,
	Array
} JSONKind;

typedef struct {
} JSONString;

typedef struct {
} JSONNumber;

typedef struct {
} JSONBoolean;

typedef struct {
} JSONNull;

typedef struct {
} JSONObject;

typedef struct {
} JSONArray;

typedef struct {
	JSONKind discriminant;
	union {
		JSONString string;
		JSONNumber number;
		JSONBoolean boolean;
		JSONNull null;
		JSONObject object;
		JSONArray array;
	} payload;
} JSONValue;

#endif // JSON_H
