
private string calculerCheckSum(string message)
{
	return calculerCheckSum(Encoding.Default.GetBytes(message));
}


private string calculerCheckSum(byte[] messageBytes)
{
	/* calcul du checksum */

	byte checksum = 0;

	int i = 0;

	for (i = 0; i < messageBytes.Length; i++)
	{
		checksum ^= messageBytes[i];
	}

	// conversion du checksum en chaine Hexa.

	string checksumHex = checksum.ToString("X2");

	/* FIN - calcul du checksum */

	return checksumHex;
}