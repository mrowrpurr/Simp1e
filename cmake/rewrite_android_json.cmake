# Read the JSON file
file(READ ${JSON_FILE} JSON_CONTENT)

# Modify the JSON content
string(REGEX REPLACE "(\"application-binary\": \")[^\"]*(\")" "\\1${NEW_VALUE}\\2" JSON_CONTENT_MODIFIED "${JSON_CONTENT}")

# Write the modified JSON content back to the file
file(WRITE ${JSON_FILE} "${JSON_CONTENT_MODIFIED}")
