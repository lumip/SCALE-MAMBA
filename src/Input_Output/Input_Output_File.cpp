
#include "Input_Output_File.h"
#include "Exceptions/Exceptions.h"

#include <istream>

std::vector<std::streampos> Input_Stream_File::build_file_index(std::istream& stream)
{
  std::vector<std::streampos> positions;
  while (stream)
  {
    positions.push_back(stream.tellg());
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  positions.push_back(stream.tellg());
  stream.clear();
  stream.seekg(0, std::ios::beg);

  return positions;
}

void Input_Stream_File::seek_to_channel(unsigned int channel)
{
  if (channel >= line_current_positions.size() - 1)
    throw IO_Error("attempted to read from unavailable channel");
  std::streampos pos = line_current_positions[channel];

  if (pos >= line_start_positions[channel + 1])
    throw IO_Error("the channel ran out of inputs");

  stream.clear();
  stream.seekg(pos, std::ios::beg);
  if (!stream)
    throw IO_Error("unspecified IO error (stream invalid)");
}

void Input_Stream_File::seek_to_next(unsigned int channel)
{
  while (!stream.eof() && stream.tellg() < line_start_positions[channel + 1])
  {
    int c = stream.peek();
    if (std::isdigit(c))
      break;
    stream.get();
  }
  line_current_positions[channel] = stream.tellg();
}

long Input_Output_File::open_channel(unsigned int)
{
  return 0;
}

void Input_Output_File::close_channel(unsigned int)
{
  ;
}

gfp Input_Output_File::private_input_gfp(unsigned int channel)
{
  gfp y;
  y.assign(private_inputs_file.read_from_channel<word>(channel));
  return y;
}

void Input_Output_File::private_output_gfp(const gfp &output, unsigned int channel)
{
  cout << "Output channel " << channel << " : ";
  output.output(cout, true);
  cout << endl;
}

gfp Input_Output_File::public_input_gfp(unsigned int channel)
{
  gfp y;
  y.assign(public_inputs_file.read_from_channel<word>(channel));

  // Important to have this call in each version of public_input_gfp
  Update_Checker(y, channel);

  return y;
}

void Input_Output_File::public_output_gfp(const gfp &output, unsigned int channel)
{
  cout << "Output channel " << channel << " : ";
  output.output(std::cout, true);
  cout << endl;
}

long Input_Output_File::public_input_int(unsigned int channel)
{
  long x(public_inputs_file.read_from_channel<long>(channel));

  // Important to have this call in each version of public_input_gfp
  Update_Checker(x, channel);

  return x;
}

void Input_Output_File::public_output_int(const long output, unsigned int channel)
{
  cout << "Output channel " << channel << " : " << output;
  cout << " = 0x" << hex << output << dec << endl;
}

void Input_Output_File::output_share(const Share&, unsigned int)
{
  throw not_implemented();
}

Share Input_Output_File::input_share(unsigned int)
{
  throw not_implemented();
}

void Input_Output_File::trigger(Schedule &schedule)
{
  printf("Restarting\n");

  // Load new schedule file program streams, using the original
  // program name
  //
  // Here you could define programatically what the new
  // programs you want to run are, by directly editing the
  // public variables in the schedule object.
  unsigned int nthreads= schedule.Load_Programs();
  if (schedule.max_n_threads() < nthreads)
    {
      throw Processor_Error("Restart requires more threads, cannot do this");
    }
}

void Input_Output_File::debug_output(const stringstream &ss)
{
  std::cout << ss.str();
}

void Input_Output_File::crash(unsigned int PC, unsigned int thread_num)
{
  printf("Crashing in thread %d at PC value %d\n", thread_num, PC);
  throw crash_requested();
}
