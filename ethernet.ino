boolean initialEthernet(){
    Serial.println("\n[getDHCPandDNS]");
  //   Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup()){
    Serial.println("DHCP failed");
    return false;
  }
  ether.printIp("My IP: ", ether.myip);
  // ether.printIp("Netmask: ", ether.mymask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);

  if (!ether.dnsLookup(website)){
    Serial.println("DNS failed");
    return false;
  }
  ether.printIp("Server: ", ether.hisip);
  timer = - REQUEST_RATE; // start timing out right away
  return true;
}
