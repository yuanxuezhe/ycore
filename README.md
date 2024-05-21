# yui
my ui library

yum install centos-release-scl -y
yum install devtoolset-11 -y
scl enable devtoolset-11 bash

wget https://cdn.mysql.com//Downloads/Connector-ODBC/8.4/mysql-connector-odbc-8.4.0-1.el7.x86_64.rpm
rpm -ivh mysql-connector-odbc-8.4.0-1.el7.x86_64.rpm

curl -s https://packagecloud.io/install/repositories/rabbitmq/erlang/script.rpm.sh | sudo bash
curl -s https://packagecloud.io/install/repositories/rabbitmq/rabbitmq-server/script.rpm.sh | sudo bash

yum list |grep erlang
yum list |grep rabbit

yum -y install rabbitmq-server
systemctl start rabbitmq-server
rabbitmq-plugins enable rabbitmq_management
rabbitmqctl status

firewall-cmd --permanent --add-port=15672/tcp
firewall-cmd --permanent --add-port=5672/tcp
systemctl restart firewalld.service

rabbitmqctl add_user admin admin
rabbitmqctl set_permissions -p / admin "." "." ".*"
rabbitmqctl set_user_tags admin administrator
