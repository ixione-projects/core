#ifndef JSON_H
#define JSON_H

typedef enum {
	String,
	Number,
	Boolean,
	Null,
	Object,
	Array
} JsonKind;

typedef struct {
} JsonString;

typedef struct {
} JsonNumber;

typedef struct {
} JsonBoolean;

typedef struct {
} JsonNull;

typedef struct {
} JsonObject;

typedef struct {
} JsonArray;

typedef struct {
	JsonKind discriminant;
	union {
		JsonString string;
		JsonNumber number;
		JsonBoolean boolean;
		JsonNull null;
		JsonObject object;
		JsonArray array;
	} payload;
} JsonValue;

#endif // JSON_H
