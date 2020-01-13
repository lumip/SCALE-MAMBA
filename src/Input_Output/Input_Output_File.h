/*
Copyright (c) 2017, The University of Bristol, Senate House, Tyndall Avenue, Bristol, BS8 1TH, United Kingdom.
Copyright (c) 2019, COSIC-KU Leuven, Kasteelpark Arenberg 10, bus 2452, B-3001 Leuven-Heverlee, Belgium.

All rights reserved
*/
#ifndef _InputOutputFile
#define _InputOutputFile


#include "Input_Output_Base.h"

#include <fstream>
#include <istream>

class Input_Stream_File
{
private:
    std::ifstream stream;
    std::vector<std::streampos> line_start_positions;
    std::vector<std::streampos> line_current_positions;

    void seek_to_channel(unsigned int channel);
    void seek_to_next(unsigned int channel);
    static std::vector<std::streampos> build_file_index(std::istream& stream);

public:
    Input_Stream_File(const string& filename)
        : stream(filename)
        , line_start_positions(build_file_index(stream))
        , line_current_positions(line_start_positions)
    {
      ;
    }

    template<typename T>
    T read_from_channel(unsigned int channel)
    {
      seek_to_channel(channel);

      T x;
      stream >> x;

      seek_to_next(channel);

      return x;
    }
};

class Input_Output_File : public Input_Output_Base
{
private:
    Input_Stream_File private_inputs_file;
    Input_Stream_File public_inputs_file;

public:
  Input_Output_File(const std::string& private_inputs_file, const std::string& public_inputs_file)
      : Input_Output_Base()
      , private_inputs_file(private_inputs_file)
      , public_inputs_file(public_inputs_file)
  {
    ;
  }

  virtual long open_channel(unsigned int channel) override;
  virtual void close_channel(unsigned int channel) override;

  virtual gfp private_input_gfp(unsigned int channel) override;
  virtual void private_output_gfp(const gfp &output, unsigned int channel) override;

  virtual void public_output_gfp(const gfp &output, unsigned int channel) override;
  virtual gfp public_input_gfp(unsigned int channel) override;

  virtual void public_output_int(const long output, unsigned int channel) override;
  virtual long public_input_int(unsigned int channel) override;

  virtual void output_share(const Share &S, unsigned int channel) override;
  virtual Share input_share(unsigned int channel) override;

  virtual void trigger(Schedule &schedule) override;

  virtual void debug_output(const std::stringstream &ss) override;

  virtual void crash(unsigned int PC, unsigned int thread_num) override;
};

#endif
