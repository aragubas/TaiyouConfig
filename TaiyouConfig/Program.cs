namespace TaiyouConfig;

enum TaiyouConfigTokenType {
    NamespaceDeclaration, NamespaceEndStatement, TypeDeclaration, 
    KeyName, KeyValue
}

struct TaiyouConfigToken 
{
    public TaiyouConfigTokenType Type { get; set; }
    public string Data { get; set; }
    public uint Line { get; set; }
    public uint Column { get; set; }
}

class Program
{
    static void Main(string[] args)
    {
        string specFilePath = "./Example Files/Settings.tcsp";
        string specFileText = File.ReadAllText(specFilePath);

        string specWithoutComments = RemoveCommentOrEmptyLines(specFileText);
        List<TaiyouConfigToken> tokens = TokenizeString(specWithoutComments);
        
        Console.WriteLine("Tokens:");
        string TokensListByType = "";
        foreach(TaiyouConfigToken token in tokens)
        {
            Console.WriteLine($"Type: {token.Type}\n\tData: '{token.Data}\n\t Line {token.Line}:{token.Column}'");
            TokensListByType += $"{token.Type} ";
        }

        Console.WriteLine($"List by type:\n{TokensListByType.Trim()}");
    }

    static string RemoveCommentOrEmptyLines(string input)
    {
        string finalString = "";

        foreach (string line in input.Split('\n')) {
            if (line.StartsWith("#") || string.IsNullOrWhiteSpace(line)) {
                continue;
            }

            finalString += $"{line}\n";
        }

        return finalString.TrimEnd();
    }

    static List<TaiyouConfigToken> TokenizeString(string input) 
    {
        List<TaiyouConfigToken> tokens = new();

        bool isInNamespaceDeclaration = false;
        string namespaceDeclarationName = "";
        bool typeDeclared = false;
        bool isDeclaringType = false;
        string typeName = "";
        bool isDeclaringKeyName = false;
        bool isDeclaringValue = false;
        string keyName = "";
        string keyValue = "";

        uint currentLine = 0;
        uint currentColumn = 0;
        foreach(Char character in input + '\n') // Append new line to end to fix last token not being declared
        {
            if (character == '\n')
            {
                currentLine++;
                currentColumn = 0;
            }else {
                currentColumn++;
            }
            
            if (character == '@' && !isInNamespaceDeclaration && !typeDeclared && !isDeclaringType)
            {
                isInNamespaceDeclaration = true;
                continue;
            }

            // Finish declaring namespace
            if (isInNamespaceDeclaration && character == '\n')
            {
                isInNamespaceDeclaration = false;
                
                // Token is namespace end
                if (namespaceDeclarationName.ToLower() == "end")
                {
                    tokens.Add(new TaiyouConfigToken() {
                        Type = TaiyouConfigTokenType.NamespaceEndStatement,
                        Line = currentLine,
                        Column = currentColumn
                    });
                } else 
                { // Otherwise, it's a namespace declaration
                    tokens.Add(new TaiyouConfigToken() {
                        Type = TaiyouConfigTokenType.NamespaceDeclaration,
                        Data = namespaceDeclarationName,
                        Line = currentLine,
                        Column = currentColumn
                    });
                }
                
                namespaceDeclarationName = "";
                continue;
            }

            if (isInNamespaceDeclaration) {
                namespaceDeclarationName += character;
                continue;
            }

            // Get KeyName and KeyValue
            if (typeDeclared)
            {
                if (character == '\n' && isDeclaringValue)
                {
                    isDeclaringValue = false;
                    typeDeclared = false;

                    tokens.Add(new TaiyouConfigToken() {
                        Type = TaiyouConfigTokenType.KeyValue,
                        Data = keyValue,
                        Line = currentLine,
                        Column = currentColumn
                    });

                    keyValue = "";
                    continue;
                }

                if (isDeclaringValue)
                {
                    keyValue += character;
                    continue;
                }
                
                // Finish declaring KeyName
                if (character == '=' && isDeclaringKeyName)
                {
                    isDeclaringKeyName = false;
                    isDeclaringValue = true;

                    tokens.Add(new TaiyouConfigToken() {
                        Type = TaiyouConfigTokenType.KeyName,
                        Data = keyName,
                        Line = currentLine,
                        Column = currentColumn
                    });
                    
                    keyName = "";
                    continue;
                }
                
                // Start declaring keyName
                if (!isDeclaringKeyName && !isDeclaringValue)
                {
                    isDeclaringKeyName = true;
                    keyName += character;
                    continue;
                }

                if (isDeclaringKeyName)
                {
                    keyName += character;
                    continue;
                }
            }

            // Start declaring type
            if (!isDeclaringType && character != ' ') 
            {
                isDeclaringType = true;
                typeName += character;
                continue;
            }

            // End declaring type
            if (isDeclaringType && character == ' ')
            {
                isDeclaringType = false;

                tokens.Add(new TaiyouConfigToken() {
                    Type = TaiyouConfigTokenType.TypeDeclaration,
                    Data = typeName,
                    Line = currentLine,
                    Column = currentColumn
                });

                typeName = "";
                typeDeclared = true;
                continue;
            }

            if (isDeclaringType)
            {
                typeName += character;
                continue;
            }
        }

        return tokens;
    }
}
