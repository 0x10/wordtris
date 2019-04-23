require 'json'
require 'net/http'

class MultiSet
  attr_accessor :set
  def initialize(set)
    @set = set
  end
  # intersection
  def &(other)
    @set & other.set
  end
  # difference
  def -(other)
    @set - other.set
  end
  # union
  def |(other)
    @set | other.set
  end
end



class WordlistGenerator
	def initialize
	end

	def start(lang, category, output_file, only_legal=true, pageid="", incl_subcat=false)
      continue = ""
      done = false
      while !done do
        if category.length > 0
          url = "https://"+lang+".wiktionary.org/w/api.php?action=query&list=categorymembers&cmtitle=Category:"+category+"&cmlimit=500&cmcontinue="+continue+"&format=json"
        else
          url = "https://"+lang+".wiktionary.org/w/api.php?action=query&list=categorymembers&cmpageid="+pageid+"&cmlimit=500&cmcontinue="+continue+"&format=json"
        end
        puts "url = " + url
        uri = URI(url)
        response = Net::HTTP.get(uri)
          json_response = JSON.parse(response)
        if json_response.key?("continue")
          continue = json_response['continue']['cmcontinue']
        else
          done = true
        end
        if json_response.key?("query") && json_response['query'].key?("categorymembers")
          arr = json_response['query']['categorymembers']
          arr.each do |word|
            if (!only_legal) || is_allowed( word['title'] )
              #puts word['title']
              if output_file != ""
                open(output_file, 'a') do |f|
                  f.puts word['title']
                end
              end
            end
            if incl_subcat && word['ns'] > 0
              start( lang, "", output_file, true, word['pageid'].to_s, true )
            end
          end
        end
      end
	end

    def is_allowed( word )
      if word.length >= 3 && word.length <= 10 && word.match(/^[[:alpha:]]+$/)
        true
      else
        false
      end
    end

    def filter_words( list_input, list_blacklist, write_to="" )
      bl = File.readlines(list_blacklist)
      input = File.readlines(list_input)
      f = File.open( write_to, "w")
      bl_set = MultiSet.new( bl )
      input_set = MultiSet.new( input )
      filtered_set = input_set - bl_set
      f << filtered_set.join("")
      f.close
    end

    def join_words( list_input, list_blacklist, write_to="" )
      bl = File.readlines(list_blacklist)
      input = File.readlines(list_input)
      f = File.open( write_to, "w")
      bl_set = MultiSet.new( bl )
      input_set = MultiSet.new( input )
      filtered_set = input_set | bl_set
      f << filtered_set.join("")
      f.close
    end
end
=begin
x = MultiSet.new(["Hallo", "Welt", "Du","arsch"])
y = MultiSet.new(["Hallo", "Arsch", "hans"])

p x - y # [2,2,4]
p x & y # [1,3,5,6]
p x | y # [1,2,3,4,5,6]

["Welt", "Du", "arsch"]
["Hallo"]
["Hallo", "Welt", "Du", "arsch", "Arsch", "hans"]
=end

de = WordlistGenerator.new
de.start("de", "", "acro.txt", false, "57787" )

de.start("de", "Substantiv_f_(Deutsch)", "subs_f.txt" )
de.start("de", "Substantiv_m_(Deutsch)", "subs_m.txt" )
de.start("de", "Substantiv_n_(Deutsch)", "subs_n.txt" )
de.start("de", "Pluraletantum_(Deutsch)", "plu_singl_sub.txt")
de.start("de", "Singularetantum_(Deutsch)", "plu_singl_sub.txt")

de.filter_words( "subs_f.txt", "acro.txt", "subs_f_filtered.txt" )
de.filter_words( "subs_m.txt", "acro.txt", "subs_m_filtered.txt" )
de.filter_words( "subs_n.txt", "acro.txt", "subs_n_filtered.txt" )
de.filter_words( "plu_singl_sub.txt", "acro.txt", "subs_pl_sing_filtered.txt" )

de.join_words( "subs_f_filtered.txt", "subs_m_filtered.txt", "subs_mf_filtered.txt" )
de.join_words( "subs_mf_filtered.txt", "subs_n_filtered.txt", "subs_mnf_filtered.txt" )
de.join_words( "subs_mnf_filtered.txt", "subs_pl_sing_filtered.txt", "subs.txt" )

de.start("de", "Verb_(Deutsch)", "verbs_ad.txt" )
de.start("de", "Adjektiv_(Deutsch)", "verbs_ad.txt" )

de.start("de", "Numerale_(Deutsch)", "num_art_gruss.txt" )
de.start("de", "Artikel_(Deutsch)", "num_art_gruss.txt" )
de.start("de", "", "num_art_gruss.txt", true, "58298" ) # grussformel

de.start("de", "Pronomen_(Deutsch)", "adverb_pronomen.txt", true, "", true )
de.start("de", "Adverb_(Deutsch)", "adverb_pronomen.txt", true, "", true )

de.join_words( "adverb_pronomen.txt", "verbs_ad.txt", "ad_pro_verb.txt" )
de.join_words( "num_art_gruss.txt", "ad_pro_verb.txt", "num_ad_art_pro_verb.txt" )
de.join_words( "subs.txt", "num_ad_art_pro_verb.txt", "xlarge.txt" )
