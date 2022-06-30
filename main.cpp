#include "Header.h"
//#include <QSysinfo>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;


static string DecimalToOctal(int dec) {
    if (dec < 1) return "0";

    string octStr = "";

    while (dec > 0)
    {
        octStr = octStr.insert(0, to_string(dec % 8));
        dec /= 8;
    }

    return octStr;
}

static string ASCIIToOctal(string str) {
    string oct = "";
    int strLen = str.length();

    for (int i = 0; i < strLen; ++i)
    {
        string cOct = DecimalToOctal(str[i]);
        int cOctLen = cOct.length();

        if (cOctLen < 3)
            for (size_t j = 0; j < (3 - cOctLen); j++)
                cOct = cOct.insert(0, "0");
        if (cOct == "000")
            cout << "error" << str[i] << endl;
        /*
        char a = cOct[0], b = cOct[1], c = cOct[2];
        int ia = a - '0',ib=b-'0',ic=c-'0';
        cOct.clear();
        cOct+= ia + '0', cOct+=ib+'0', cOct+= ic+'0';
        */
        oct += cOct;
    }

    return oct;
}



string ListIpAddresses() {

    IP_ADAPTER_ADDRESSES* adapter_addresses(NULL);
    IP_ADAPTER_ADDRESSES* adapter(NULL);

    DWORD adapter_addresses_buffer_size = 16 * 1024;

    // Get adapter addresses
    for (int attempts = 0; attempts != 3; ++attempts) {
        adapter_addresses = (IP_ADAPTER_ADDRESSES*)malloc(adapter_addresses_buffer_size);

        DWORD error = ::GetAdaptersAddresses(AF_UNSPEC,
            GAA_FLAG_SKIP_ANYCAST |
            GAA_FLAG_SKIP_MULTICAST |
            GAA_FLAG_SKIP_DNS_SERVER |
            GAA_FLAG_SKIP_FRIENDLY_NAME,
            NULL,
            adapter_addresses,
            &adapter_addresses_buffer_size);

        if (ERROR_SUCCESS == error) {
            break;
        }
        else if (ERROR_BUFFER_OVERFLOW == error) {
            // Try again with the new size
            free(adapter_addresses);
            adapter_addresses = NULL;
            continue;
        }
        else {
            // Unexpected error code - log and throw
            free(adapter_addresses);
            adapter_addresses = NULL;
            return 0;
        }
    }


    string itog = "";
    // Iterate through all of the adapters
    for (adapter = adapter_addresses; NULL != adapter; adapter = adapter->Next) {
        // Skip loopback adapters
        if (IF_TYPE_SOFTWARE_LOOPBACK == adapter->IfType) continue;
        int i;
        printf("[ADAPTER]: %S\n", adapter->Description);
        printf("[NAME]:    %S\n", adapter->FriendlyName);
        //самописный модуль
        typedef wchar_t* PWCHAR;
        PWCHAR S1 = (adapter->Description), S2 = (adapter->FriendlyName), S3;
        wstring str1(S1), str2(S2);
        itog += "[ADAPTER]:";
        for (i = 0; i < str1.length(); i++)
            itog += str1[i];
        itog += "\n[NAME]:";
        for (i = 0; i < str2.length(); i++)
            itog += str2[i];
        //cout<<str;

        // Parse all IPv4 addresses
        for (IP_ADAPTER_UNICAST_ADDRESS* address = adapter->FirstUnicastAddress; NULL != address; address = address->Next) {
            auto family = address->Address.lpSockaddr->sa_family;
            if (AF_INET == family) {
                SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(address->Address.lpSockaddr);
                char str_buffer[16] = { 0 };
                inet_ntop(AF_INET, &(ipv4->sin_addr), str_buffer, 16);
                printf("[IP]:      %s\n", str_buffer);
                itog += "\n[IP]:";
                for (i = 0; i < strlen(str_buffer); i++)
                    itog += str_buffer[i];
            }
        }
        itog += "\n";
    }
    free(adapter_addresses);
    adapter_addresses = NULL;
    return itog;
}


const char* GetOsVersionName()
{

    if (IsWindows10OrGreater())
    {
        return "Windows10OrGreater\n";
    }
    if (IsWindows8Point1OrGreater())
    {
        return "Windows8Point1OrGreater\n";
    }
    if (IsWindows8OrGreater())
    {
        return "Windows8OrGreater\n";
    }
    if (IsWindows7SP1OrGreater())
    {
        return "Windows7SP1OrGreater\n";
    }
    if (IsWindows7OrGreater())
    {
        return "Windows7OrGreater\n";
    }
    if (IsWindowsVistaSP2OrGreater())
    {
        return "VistaSP2OrGreater\n";
    }
    if (IsWindowsVistaSP1OrGreater())
    {
        return "VistaSP1OrGreater\n";
    }
    if (IsWindowsVistaOrGreater())
    {
        return "VistaOrGreater\n";
    }
    if (IsWindowsXPSP3OrGreater())
    {
        return "XPSP3OrGreater\n";
    }
    if (IsWindowsXPSP2OrGreater())
    {
        return "XPSP2OrGreater\n";
    }
    if (IsWindowsXPSP1OrGreater())
    {
        return "XPSP1OrGreater\n";
    }
    if (IsWindowsXPOrGreater())
    {
        return "XPOrGreater\n";
    }
    if (IsWindowsServer())
    {
        return "Server\n";
    }
    else
    {
        return "Client\n";
    }
}


void _tmain(void)
{
    string itogi = "";
    //
    TCHAR buffer[256] = TEXT("");
    TCHAR szDescription[8][32] = { TEXT("NetBIOS"),
        TEXT("DNS hostname"),
        TEXT("DNS domain"),
        TEXT("DNS fully-qualified"),
        TEXT("Physical NetBIOS"),
        TEXT("Physical DNS hostname"),
        TEXT("Physical DNS domain"),
        TEXT("Physical DNS fully-qualified") };
    int cnf = 0, i = 0;
    DWORD dwSize = _countof(buffer);

    for (cnf = 0; cnf < ComputerNameMax; cnf++)
    {
        if (!GetComputerNameEx((COMPUTER_NAME_FORMAT)cnf, buffer, &dwSize))
        {
            _tprintf(TEXT("GetComputerNameEx failed (%d)\n"), GetLastError());
            return;
        }
        else _tprintf(TEXT("%s: %s\n"), szDescription[cnf], buffer);
        itogi += "[";
        for (i = 0; i < 32; i++)
        {
            if (szDescription[cnf][i] == '\0')
                break;
            itogi += szDescription[cnf][i];
        }
        itogi += "]:";
        wstring test(&buffer[0]);
        string test2(test.begin(), test.end());
        itogi += test2;
        itogi += "\n";
        dwSize = _countof(buffer);
        ZeroMemory(buffer, dwSize);
    }


    //info_about system
    _SYSTEM_INFO sysinfo;   //https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-system_info
    GetNativeSystemInfo(&sysinfo);
    unsigned long s1 = sysinfo.dwNumberOfProcessors, s2 = sysinfo.dwProcessorType, s3 = sysinfo.wProcessorArchitecture, s4 = sysinfo.wProcessorLevel, s5 = sysinfo.wProcessorRevision;
    cout << "\nNumOfProc - " << s1 << "\nProcType - " << s2 << "\nArch - " << s3 << "\nProcLvl- " << s4
        << "\nProcRevis - " << s5 << endl;
    itogi += "[NumOfProc]:" + to_string(s1) + "\n[ProcType]:" + to_string(s2) + "\n[Arch]:" + to_string(s3);
    itogi += "\n[ProcLvl]:" + to_string(s4) + "\n[ProcRevis]:" + to_string(s5);
    itogi += "\n";

    //system time
    SYSTEMTIME st, lt;

    GetSystemTime(&st);
    GetLocalTime(&lt);

    cout << "The system time is: " << st.wHour << ":" << st.wMinute << endl;
    cout << " The local time is: " << lt.wHour << ":" << lt.wMinute << endl;

    itogi += "[SYS_TIME]:" + to_string(st.wHour) + "-" + to_string(st.wMinute);
    itogi += "\n[LOC_TIME]:" + to_string(lt.wHour) + "-" + to_string(lt.wMinute) + "\n";

    //https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemwindowsdirectoryw
    TCHAR bufferer[256] = TEXT("");
    DWORD dwSizee = _countof(bufferer);
    GetSystemWindowsDirectoryW(bufferer, dwSizee);
    cout << "\nPath - " << bufferer << endl;
    wstring test3(&bufferer[0]);
    string test4(test3.begin(), test3.end());
    itogi += "[PATH]:" + test4 + "\n";

    //https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getphysicallyinstalledsystemmemory
    unsigned long long physicalMemory = 0;
    ULONGLONG ram;
    GetPhysicallyInstalledSystemMemory(&ram);
    int rams = ram / 1024;
    cout << "\n RAM mb = " << rams << endl;
    itogi += "[RAM]:" + to_string(rams) + "\n";

    //Version OS
    const char* osVersionName = GetOsVersionName();
    cout << "Running under Windows %s\n" << osVersionName << endl;
    itogi += "[WIN_VER]:";
    itogi += osVersionName;


    //take IP and adapters
    string str = ListIpAddresses();

    itogi += str;


    //TCP
    const char* buf = itogi.c_str();
    WSADATA wsData;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (erStat != 0) {
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
    }
    else
        cout << "WinSock initialization is OK" << endl;
    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

    if (ClientSock == INVALID_SOCKET) {
        cout << "Error initialization socket # " << WSAGetLastError() << endl;
        closesocket(ClientSock);
        WSACleanup();
    }
    else
        cout << "Server socket initialization is OK" << endl;

    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
    if (erStat <= 0) {
        cout << "Error in IP translation to special numeric format" << endl;
    }
    sockaddr_in servInfo;

    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;	  // Server's IPv4 after inet_pton() function
    servInfo.sin_port = htons(1200);

    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if (erStat != 0) {
        cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock);
        WSACleanup();
    }
    else
        cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
        << endl;
    while (true)
    {
        short packet_size = 0;
        packet_size = send(ClientSock, buf, strlen(buf), 0);
        if (packet_size == SOCKET_ERROR)
        {
            cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSock);
            WSACleanup();
            break;
        }
    }

    closesocket(ClientSock);
    WSACleanup();

    cout << "\n\nTEST\n" << itogi << endl;
    std::cout << dec << itogi[0] << '\n';
    std::cout << hex << itogi[0] << '\n';
    std::cout << oct << itogi[0] << '\n';


    
    /// value - строка чисел, где каждая тройка чисел - представление буквы в OCT по ASCII
    ///
    /// </summary>
    /// <param name=""></param>
    string value = ASCIIToOctal(itogi);
    string tik = "";
    int ch_f1 = 10 , chislo = 0;
    for (int j = 0; j < value.length(); j++)
    {
        tik += value[j];
        cout << "Tik >> " << tik << endl;
        int cock = stoi(tik);
        cout << "STik >> " << cock << endl;
        if (tik.length() % ch_f1 == 0)
        {
           // cout << "Tik >> " << tik << endl;
            //cout << "STik >> " << cock << endl;
            tik = "";
        }
    }
    
    cout << value << endl;

}


//int main()