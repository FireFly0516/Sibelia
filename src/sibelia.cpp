//****************************************************************************
//* Copyright (c) 2012 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#include <tclap/CmdLine.h>
#include "util.h"
#include "postprocessor.h"

int main(int argc, char * argv[])
{	
	signal(SIGINT, SignalHandler);
	signal(SIGABRT, SignalHandler);	
	signal(SIGTERM, SignalHandler);

	std::stringstream parsets;		
	const std::string parameterSetNameArray[] = {"loose", "fine"};
	std::vector<std::string> parameterSetName(parameterSetNameArray, parameterSetNameArray + sizeof(parameterSetNameArray) / sizeof(parameterSetNameArray[0]));
	std::map<std::string, std::vector<std::pair<int, int> > > defaultParameters;
	defaultParameters["loose"] = LooseStageFile();
	defaultParameters["fine"] = FineStageFile();

	try
	{  
		TCLAP::CmdLine cmd("Program for finding syteny blocks in closely related genomes", ' ', "3.0.0");
		TCLAP::ValueArg<unsigned int> maxIterations("i",
			"maxiterations",
			"Maximum number of iterations during a stage of simplification, default = 4.",
			false,
			4,
			"integer",
			cmd);

		TCLAP::SwitchArg comparativeFlag("",
			"comparative",
			"internal flag used by C-Sibelia",
			cmd,
			false);

		TCLAP::ValueArg<std::string> tempFileDir("t",
			"tempdir",
			"Directory where temporary files are stored",
			false,
			".",
			"dir name",
			cmd);

		TCLAP::ValueArg<std::string> stageFile("k",
			"stagefile",
			"File that contains manually chosen simplifications parameters. See USAGE file for more information.",
			false,
			"",
			"file name");

		TCLAP::SwitchArg hierarchyPicture("v",
			"visualize",
			"Draw circos diagram with blocks at different stages",
			cmd,
			false);

		TCLAP::SwitchArg graphFile("g",
			"graphfile",
			"Output resulting condensed de Bruijn graph (in dot format), default = not set.",
			cmd,
			false);

		TCLAP::SwitchArg sequencesFile("q",
			"sequencesfile",
			"Output sequences of synteny blocks (FASTA format), default = not set.",
			cmd,
			false);		

		std::string description = std::string("Parameters set, used for the simplification. ") +
			std::string("Option \"loose\" produces fewer blocks, but they are larger (\"fine\" is opposite).");
		TCLAP::ValuesConstraint<std::string> allowedParametersVals(parameterSetName);
		TCLAP::ValueArg<std::string> parameters("s",
			"parameters",
			description,
			false,
			parameterSetName[0],
			&allowedParametersVals);

		TCLAP::ValueArg<unsigned int> minBlockSize("m",
			"minblocksize",
			"Minimum size of a synteny block, default value = 5000 BP.",
			false,
			5000,
			"integer",
			cmd);

		TCLAP::SwitchArg sharedOnly("a",
			"sharedonly",
			"Output only blocks that occur exactly once in each input sequence.",			
			cmd,
			false);

		TCLAP::SwitchArg inRAM("r",
			"inram",
			"Perform all computations in RAM, don't create temp files.",
			cmd,
			false);

		TCLAP::UnlabeledMultiArg<std::string> fileName("filenames",
			"FASTA file(s) with nucleotide sequences.",
			true,
			"fasta files with genomes",
			cmd);

		TCLAP::ValueArg<std::string> outFileDir("o",
			"outdir",
			"Directory where output files are written",
			false,
			".",
			"dir name",
			cmd);		

		cmd.xorAdd(parameters, stageFile);
		cmd.parse(argc, argv);

		std::vector<std::pair<int, int> > stage;
		if(parameters.isSet())
		{
			stage = defaultParameters[parameters.getValue()];
		}
		else
		{
			stage = ReadStageFile(stageFile.getValue());
		}
		
		int trimK = INT_MAX;
		size_t totalSize = 0;
		std::set<size_t> referenceChrId;
		bool hierarchy = hierarchyPicture.isSet();
		bool comparative = comparativeFlag.isSet();
		if(comparative && (fileName.end() - fileName.begin()) != 2)
		{
			throw std::runtime_error("In comparative mode only two FASTA files are acceptable");
		}

		std::vector<SyntenyFinder::FASTARecord> chrList;
		for(std::vector<std::string>::const_iterator it = fileName.begin(); it != fileName.end(); it++)
		{
			SyntenyFinder::FASTAReader reader(*it);
			if(!reader.IsOk())
			{
				throw std::runtime_error(("Cannot open file " + *it).c_str());
			}

			reader.GetSequences(chrList);
			if(it == fileName.begin() && comparative)
			{
				for(size_t i = 0; i < chrList.size(); i++)
				{
					referenceChrId.insert(chrList[i].GetId());
				}
			}
		}
		
		for(size_t i = 0; i < chrList.size(); i++)
		{
			totalSize += chrList[i].GetSequence().size();
		}

		if(totalSize > SyntenyFinder::MAX_INPUT_SIZE)
		{
			throw std::runtime_error("Input is larger than 1 GB, can't proceed");
		}
		
		std::vector<std::vector<SyntenyFinder::BlockInstance> > history(stage.size() + 1);
		std::string tempDir = tempFileDir.isSet() ? tempFileDir.getValue() : outFileDir.getValue();		
		std::auto_ptr<SyntenyFinder::BlockFinder> finder(inRAM.isSet() ? new SyntenyFinder::BlockFinder(chrList) : new SyntenyFinder::BlockFinder(chrList, tempDir));
		SyntenyFinder::Postprocessor processor(chrList, minBlockSize.getValue());
		for(size_t i = 0; i < stage.size(); i++)
		{
			trimK = std::min(trimK, stage[i].first);
			if(hierarchy || comparative)
			{
				finder->GenerateSyntenyBlocks(stage[i].first, trimK, stage[i].first, history[i], sharedOnly.getValue());
				processor.GlueStripes(history[i]);
			}

			std::cout << "Simplification stage " << i + 1 << " of " << stage.size() << std::endl;
			std::cout << "Enumerating vertices of the graph, then performing bulge removal..." << std::endl;
			finder->PerformGraphSimplifications(stage[i].first, stage[i].second, maxIterations.getValue(), PutProgressChr);			
		}
				
		std::cout << "Finding synteny blocks and generating the output..." << std::endl;
		size_t lastK = std::min(stage.back().first, static_cast<int>(minBlockSize.getValue()));
		trimK = std::min(trimK, static_cast<int>(minBlockSize.getValue()));
		finder->GenerateSyntenyBlocks(lastK, trimK, minBlockSize.getValue(), history.back(), sharedOnly.getValue(), PutProgressChr);
		processor.GlueStripes(history.back());

		SyntenyFinder::OutputGenerator generator(chrList);
		SyntenyFinder::CreateOutDirectory(outFileDir.getValue());
		const std::string defaultCoordsFile = outFileDir.getValue() + "/blocks_coords.txt";
		const std::string defaultPermutationsFile = outFileDir.getValue() + "/genomes_permutations.txt";
		const std::string defaultCoverageReportFile = outFileDir.getValue() + "/coverage_report.txt";
		const std::string defaultSequencesFile = outFileDir.getValue() + "/blocks_sequences.fasta";
		const std::string defaultGraphFile = outFileDir.getValue() + "/de_bruijn_graph.dot";
		const std::string defaultCircosDir = outFileDir.getValue() + "/circos";
		const std::string defaultCircosFile = defaultCircosDir + "/circos.conf"; const std::string defaultD3File = outFileDir.getValue() + "/d3_blocks_diagram.html";		
        const std::string defaultBlocksAlignmentFile = outFileDir.getValue() + "/blocks_sequences.sam";
		generator.ListChromosomesAsPermutations(history.back(), defaultPermutationsFile);
		generator.GenerateReport(history.back(), defaultCoverageReportFile);
		generator.ListBlocksIndices(history.back(), defaultCoordsFile);
		generator.GenerateD3Output(history.back(), defaultD3File);
		if(sequencesFile.isSet())
		{
			generator.ListBlocksSequences(history.back(), defaultSequencesFile);
            generator.OutputBlocksInSAM(history.back(), defaultBlocksAlignmentFile);			
		}

		if(comparative)
		{
			processor.ImproveBlockBoundaries(history.back(), referenceChrId);
			for(size_t i = 0; i < history.size(); i++)
			{
				std::stringstream file;
				file << outFileDir.getValue() << "/blocks_coords" << i << ".txt";
				generator.ListBlocksIndices(history[i], file.str());
			}
		}

		if(!hierarchy)
		{
			generator.GenerateCircosOutput(history.back(), defaultCircosFile, defaultCircosDir);
		}
		else
		{
			generator.GenerateHierarchyCircosOutput(history, defaultCircosFile, defaultCircosDir);
		}		

		if(graphFile.isSet())
		{
			std::stringstream buffer;
			finder->SerializeCondensedGraph(lastK, buffer, PutProgressChr);
			generator.OutputBuffer(defaultGraphFile, buffer.str());
		}

		std::cout.setf(std::cout.fixed);
		std::cout.precision(2);
		std::cout << "Time elapsed: " << double(clock()) / CLOCKS_PER_SEC << " seconds" << std::endl;
	} 
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
	}
	catch (std::runtime_error & e)
	{
		std::cerr << "error: " << e.what() << std::endl;
	}
	catch(...)
	{
		SyntenyFinder::TempFile::Cleanup();
	}

	return 0;
}
