#include "Header.h"

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;


unsigned powmod(unsigned base, unsigned exp, unsigned modulo)
{
    unsigned res = 1;

    while (exp != 0)
    {
        if ((exp & 1) != 0)
        {
            res = (1ll * res * base) % modulo;
        }

        base = (1ll * base * base) % modulo;
        exp >>= 1;
    }

    return res;
}

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
    int cnf = 0, i = 0,error=0, ch_f1 = 10, chislo = 0;
    string R_I_S = "", tik = "";
    const short BUFF_SIZE = 1024;
    vector <char> servBuff(BUFF_SIZE);
    char M[2048];
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
    DWORD dwSize = _countof(buffer);
    for (cnf = 0; cnf < ComputerNameMax; cnf++)
    {
        if (!GetComputerNameEx((COMPUTER_NAME_FORMAT)cnf, buffer, &dwSize))
        {
            _tprintf(TEXT("GetComputerNameEx failed (%d)\n"), GetLastError());
            return;
        }
        else _tprintf(TEXT("%s: %s\n"), szDescription[cnf], buffer);
        R_I_S += "[";
        for (i = 0; i < 32; i++)
        {
            if (szDescription[cnf][i] == '\0')
                break;
            R_I_S += szDescription[cnf][i];
        }
        R_I_S += "]:";
        wstring test(&buffer[0]);
        string test2(test.begin(), test.end());
        R_I_S += test2;
        R_I_S += "\n";
        dwSize = _countof(buffer);
        ZeroMemory(buffer, dwSize);
    }


    //info_about system
    _SYSTEM_INFO sysinfo;   //https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-system_info
    GetNativeSystemInfo(&sysinfo);
    unsigned long s1 = sysinfo.dwNumberOfProcessors, s2 = sysinfo.dwProcessorType, s3 = sysinfo.wProcessorArchitecture, s4 = sysinfo.wProcessorLevel, s5 = sysinfo.wProcessorRevision;
    R_I_S += "[NumOfProc]:" + to_string(s1) + "\n[ProcType]:" + to_string(s2) + "\n[Arch]:" + to_string(s3);
    R_I_S += "\n[ProcLvl]:" + to_string(s4) + "\n[ProcRevis]:" + to_string(s5);
    R_I_S += "\n";

    //system time
    SYSTEMTIME st, lt;

    GetSystemTime(&st);
    GetLocalTime(&lt);

    R_I_S += "[SYS_TIME]:" + to_string(st.wHour) + "-" + to_string(st.wMinute);
    R_I_S += "\n[LOC_TIME]:" + to_string(lt.wHour) + "-" + to_string(lt.wMinute) + "\n";

    TCHAR bufferer[256] = TEXT("");
    DWORD dwSizee = _countof(bufferer);
    GetSystemWindowsDirectoryW(bufferer, dwSizee);
    wstring test3(&bufferer[0]);
    string test4(test3.begin(), test3.end());
    R_I_S += "[PATH]:" + test4 + "\n";

    unsigned long long physicalMemory = 0;
    ULONGLONG ram;
    GetPhysicallyInstalledSystemMemory(&ram);
    int rams = ram / 1024;
    R_I_S += "[RAM]:" + to_string(rams) + "\n";

    //Version OS
    const char* osVersionName = GetOsVersionName();
    R_I_S += "[WIN_VER]:";
    R_I_S += osVersionName;

    //take IP and adapters
    string str = ListIpAddresses();

    R_I_S += str;

    string value = ASCIIToOctal(R_I_S),itogi_2="";
   
    //TCP
    const char* buf = R_I_S.c_str();
    WSADATA wsData;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (erStat != 0) {
        int q = 0;
    }
    else
        int q = 0;
    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

    if (ClientSock == INVALID_SOCKET) {
        closesocket(ClientSock);
        WSACleanup();
    }
    else
        int q = 0;

    in_addr ip_to_num;//127.0.0.1
    erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
    if (erStat <= 0) {
        int q = 0;
    }
    sockaddr_in servInfo;

    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;	  // Server's IPv4 after inet_pton() function
    servInfo.sin_port = htons(1200);

    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if (erStat != 0) {
        closesocket(ClientSock);
        WSACleanup();
    }
    else
        int q = 0;

    while (true)
    {
        short packet_size = 0;
        packet_size = send(ClientSock, buf, strlen(buf), 0);
        if (packet_size == SOCKET_ERROR)
        {
            closesocket(ClientSock);
            error = 1;
            WSACleanup();
            break;
        }
        packet_size = recv(ClientSock, M, servBuff.size(), 0);
        cout << M;
        if (packet_size == SOCKET_ERROR) {
            closesocket(ClientSock);
            WSACleanup();
        }
        else
            break;
    }
    closesocket(ClientSock);
    WSACleanup();

    cout << R_I_S;

}
