|include(macro.h)

|environment(ret_address;
	verbatim on, indent +30, fill off, here on;

	minimum.lines)
	|comment<summary: ret_address (return address)>

|environment(date;
	verbatim on, indent +30, fill off, here on;

	minimum.lines)
	|comment<summary: date (date)>

|environment(address;
	verbatim on, fill off, here on, minimum.lines;

	minimum.lines)
	|comment<summary: address (who to address the letter to)>

|environment(greeting;
	minimum.lines 2;

	minimum.lines)
	|comment<summary: greeting (greeting)>

|environment(body;
	minimum.lines, fill on;

	minimum.lines)
	|comment<summary: body (body of letter)>

|environment(closing;
	minimum.lines 2, indent +30;

	minimum.lines)
	|comment<summary: closing (salutation)>

|environment(signature;
	verbatim on, minimum.lines 3, indent +30, line.length +3i;

	line.length -3i)
	|comment<summary: signature (signature)>

|environment(:document;
	;
)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)
