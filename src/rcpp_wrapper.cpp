
#include <Rcpp.h>
#include <string>
#include <iostream>
#include "adapterremoval/adrm_interface.h"
#include "renamer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sam2bed.h"
#include "bowtie2/bowtie2_interface.h"
#include "ChrDivi.h"
#include "CutSiteCount.h"

// [[Rcpp::export]]
int removeAdapter(Rcpp::CharacterVector argvs) {
	int argc=argvs.size();
	char **argv = new char* [argc];
	for(int i=0;i<argc;i++){
		int len = argvs[i].size();
		argv[i] = new char[len+1];
		strcpy(argv[i],(char *)(Rcpp::as<std::string>(argvs[i])).c_str());
//(char *)(Rcpp::as<std::string>(argvs[i])).c_str();
		//std::cout<<argv[i]<<std::endl;
	}
	//std::cout<<argc<<std::endl;

/*
        int argc=17;
        char **argv = new char* [argc];
        argv[0]="AdapterRemoval";
        argv[1]="--file1";//"--help";
        argv[2]="./data/SRR891274_1.fastq";
        argv[3]="--file2";
        argv[4]="./data/SRR891274_2.fastq";
        argv[5]="--output1";
        argv[6]="./data/SRR891274_1.fastq.clipped";
        argv[7]="--output2";
        argv[8]="./data/SRR891274_2.fastq.clipped";
        argv[9]="--basename";
        argv[10]="./data/SRR891274.clipped";
        argv[11]="--adapter1";
        argv[12]="CTGTCTCTTATACACATCTCCGAGCCCACGAGACGGACT";
        argv[13]="--adapter2";
        argv[14]="CTGTCTCTTATACACATCTGACGCTGCCGACGAGTGTAG";
        argv[15]="--threads";
        argv[16]="28";
*/
    return interface_adapterremoval_main(argc,argv);
}

// [[Rcpp::export]]
int renamer(Rcpp::List argvs) {
        std::string ipath = Rcpp::as<std::string>(argvs["inputFile"]);
        std::string opath = Rcpp::as<std::string>(argvs["outputFile"]);
        std::string filetype = Rcpp::as<std::string>(argvs["fileType"]);
        Renamer rn((char*)ipath.c_str(),(char*)opath.c_str());
        if(filetype=="fq"){
                return rn.renameFastq();
        }else if(filetype=="fa"){
                return rn.renameFasta();
        }

        return 1;
}

// [[Rcpp::export]]
int bowtie2Mapping(Rcpp::CharacterVector argvs) {
  int argc=argvs.size();
  char **argv = new char* [argc];
  for(int i=0;i<argc;i++){
    int len = argvs[i].size();
    argv[i] = new char[len+1];
    strcpy(argv[i],(char *)(Rcpp::as<std::string>(argvs[i])).c_str());

  }
  return interface_bowtie_main(argc, (const char **)argv);
}

// [[Rcpp::export]]
int bowtie2Build(Rcpp::CharacterVector argvs) {
  int argc=argvs.size();
  char **argv = new char* [argc];
  for(int i=0;i<argc;i++){
    int len = argvs[i].size();
    argv[i] = new char[len+1];
    strcpy(argv[i],(char *)(Rcpp::as<std::string>(argvs[i])).c_str());

  }
  return interface_bowtie_build_main(argc, (const char **)argv);
}



// [[Rcpp::export]]
int R_sam2bed_wrapper(Rcpp::List argvs)
{
  std::cout << "11111" << std::endl;
  std::string ipath = Rcpp::as<std::string>(argvs["samfile"]);
  std::string opath = Rcpp::as<std::string>(argvs["bedfile"]);
  int readlen = Rcpp::as<int>(argvs["readlen"]);
  std::cout << "22222" << std::endl;
  SamToBed SB((char*)ipath.c_str(), (char*)opath.c_str(), readlen);

  return(SB.sam2bed());

}

// [[Rcpp::export]]
int ChrDivi_wrapper(Rcpp::List argvs)
{
  std::cout << "Your input file will be segmented according to the chrmatin name!" << std::endl;
  std::string RIfile = Rcpp::as<std::string>(argvs["readsIfile"]);
  std::string ROfile = Rcpp::as<std::string>(argvs["readsOpath"]);

  // CHR_DIVIDE is the class name
  ChrInfoDivi CHR_DIVIDE(RIfile, ROfile);
  std::cout << "segmentation finished! Your output file path is:" << std::endl;
  std::cout << ROfile << std::endl;
  return(CHR_DIVIDE.DoDivi());

}

// [[Rcpp::export]]
int CutSiteCount_wrapper(Rcpp::List argvs)
{
  //parameters
  //string ForwReadsFile, string RevReadsFile, string MotifFile,
  //string ForwMatrixFile, string RevMatrixFile, int motif_length, int strand_length

  std::string ForwReadsFile = Rcpp::as<std::string>(argvs["ForwReadsFile"]);
  std::string RevReadsFile = Rcpp::as<std::string>(argvs["RevReadsFile"]);
  std::string MotifFile = Rcpp::as<std::string>(argvs["MotifFile"]);
  std::string ForwMatrixFile = Rcpp::as<std::string>(argvs["ForwMatrixFile"]);
  std::string RevMatrixFile = Rcpp::as<std::string>(argvs["RevMatrixFile"]);
  int motif_length = Rcpp::as<int>(argvs["motif_length"]);
  int strand_length = Rcpp::as<int>(argvs["strand_length"]);

  CutSiteCount ATAC_CSC(ForwReadsFile, RevReadsFile, MotifFile, ForwMatrixFile, RevMatrixFile, motif_length, strand_length);
  ATAC_CSC.ForwCutSiteCount();
  ATAC_CSC.RevCutSiteCount();
  return(0);

}
