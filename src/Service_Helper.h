class Service_Helper {
public:

    static bool isCorrectExtension(std::string name, std::string fileExt);

    static bool isCorrectStartName(std::string name, std::string startString);

    static bool hasHexId(std::string name);

    static bool isTriggerName(std::string name);

    static std::string generateFileName();

    static void createTarFile(std::string searchPath, std::string outputPath);

    static void getMetaData(std::string searchPath);
};
