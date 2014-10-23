#include "gmail-api.h"
#include "mail-info.h"

#include <boost/python.hpp>
#include <Python.h>

namespace
{
std::string parse_python_exception()
{
    using namespace boost::python;

    PyObject* type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
    std::string ret("Unfetchable Python error");

    if (type_ptr != NULL)
    {
        handle<> h_type(type_ptr);
        str type_pstr(h_type);
        ret = extract<std::string> (type_pstr);
    }

    if (value_ptr != NULL)
    {
        handle<> h_val(value_ptr);
        str a(h_val);
        extract<std::string> returned(a);

        if (returned.check())
        {
            ret +=  ": " + returned();
        }
        else
        {
            ret += std::string(": Unparseable Python error: ");
        }
    }

    if (traceback_ptr != NULL)
    {
        handle<> h_tb(traceback_ptr);
        object tb(import("traceback"));
        object fmt_tb(tb.attr("format_tb"));
        object tb_list(fmt_tb(h_tb));
        object tb_str(str("\n").join(tb_list));
        extract<std::string> returned(tb_str);

        if (returned.check())
        {
            ret += ": " + returned();
        }
        else
        {
            ret += std::string(": Unparseable Python traceback");
        }
    }

    return ret;
}

void RunPythonScript(int argc, char** argv, GMailInfoRequest& sender)
{
    Py_Initialize();
    PySys_SetArgv(argc, argv);
    using namespace boost::python;

    try
    {
        // Retrieve the main module.
        object main = import("__main__");

        // Retrieve the main module's namespace
        object global(main.attr("__dict__"));

        exec_file("gmail-api.py", global, global);

        object getMailInfo = global["getMail"];

        boost::python::list py_info_list = extract<boost::python::list>(getMailInfo("client_secret-home.budget.001.json"));

        MailInfoList mailList;

        for (int i = 0; i < len(py_info_list); ++i)
        {
            MailInfo mailInfo;
            object py_info = py_info_list[i];
            mailInfo.date = extract<std::string>(py_info.attr("date"));
            mailInfo.sender = extract<std::string>(py_info.attr("sender"));
            mailInfo.receiver = extract<std::string>(py_info.attr("receiver"));
            mailInfo.subject = extract<std::string>(py_info.attr("subject"));

            mailList.push_back(mailInfo);
        }

        sender.ReceiveMailInfo(mailList);
    }
    catch (error_already_set& ex)
    {
        std::string perror_str = parse_python_exception();
        std::cout << "Error in Python: " << perror_str << std::endl;
        sender.ReceiveMailInfo(MailInfoList());
    }
}

}

void GMailInfoRequest::RequestMailInfo(IMailInfoReceiver* receiver)
{
    m_receiver = receiver;

    RunPythonScript(m_argc, m_argv, *this);
}

void GMailInfoRequest::ReceiveMailInfo(const MailInfoList& list)
{
    if (m_receiver)
    {
        m_receiver->ReceiveMailInfo(list);
    }
}
