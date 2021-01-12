class CppException : public std::runtime_error {
	const std::string file;
    const std::string func;
    std::string python_info;
    
    public:
    	CppException( const std::string& msg, const std::string file_ = "", const std::string func_ = "" ) : 
        std::runtime_error(msg), file (file_), func (func_) {
            python_info = msg;
        }
        
        const std::string getFile() const { return file; }
        const std::string getFunc() const { return func; }
        const std::string getPythonInfo() const { return python_info; }
       
};
